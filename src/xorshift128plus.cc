#include <stdint.h>
#include <node.h>
#include <nan.h>

#include <cmath>
#include <iostream>

static const int kMantissaBits = 52;
static const uint64_t kMantissaMask = (1ULL << kMantissaBits) - 1;

class XorShift128Plus : public Nan::ObjectWrap {
public:
  static NAN_MODULE_INIT(Init);
private:
  static NAN_METHOD(New);
  static NAN_METHOD(Next);

  uint64_t *state;
  Nan::Persistent<v8::ArrayBuffer> buffer;
};

NAN_MODULE_INIT(XorShift128Plus::Init) {
  auto cname = Nan::New("XorShift128Plus").ToLocalChecked();
  auto ctor = Nan::New<v8::FunctionTemplate>(New);
  auto ctorInst = ctor->InstanceTemplate();

  ctor->SetClassName(cname);
  ctorInst->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(ctor, "next", Next);

  Nan::Set(target, cname, Nan::GetFunction(ctor).ToLocalChecked());
}

NAN_METHOD(XorShift128Plus::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError("Cannot call a class constructor");
  }

  if (info.Length() < 1) {
    return Nan::ThrowError("Constructor takes at least one argument");
  }

  if (info[0]->IsArrayBuffer() == false) {
    return Nan::ThrowTypeError("First argument should be an ArrayBuffer");
  }

  size_t offset = 0;

  if (info.Length() > 1) {
    if (info[1]->IsUndefined() == false) {
      if (info[1]->IsNumber() == false) {
        return Nan::ThrowTypeError("Second argument should be a number");
      }

      int64_t raw_offset = info[1]->IntegerValue();

      if (raw_offset < 0) {
        return Nan::ThrowRangeError("Offset cannot be negative");
      }

      offset = static_cast<size_t>(raw_offset);
    }
  }

  auto buffer = info[0].As<v8::ArrayBuffer>();

  if (buffer->ByteLength() < offset + 16) {
    return Nan::ThrowRangeError("Length out of range of buffer");
  }

  auto view = v8::Uint8Array::New(buffer, offset, 16);
  Nan::TypedArrayContents<uint64_t> typedData(view);

  auto me = new XorShift128Plus();

  me->buffer.Reset(buffer);
  me->state = *typedData;

  me->Wrap(info.This());
}

NAN_METHOD(XorShift128Plus::Next) {
  auto state = XorShift128Plus::Unwrap<XorShift128Plus>(info.Holder())->state;

  uint64_t x = state[0];
  uint64_t const y = state[1];
  state[0] = y;
  x ^= x << 23;
  x ^= x >> 17;
  x ^= y;
  x ^= y >> 26;
  state[1] = x;

  uint64_t mantissa = ((state[0] + state[1]) & kMantissaMask);
  auto result = Nan::New<v8::Number>(std::ldexp(mantissa, -kMantissaBits));

  info.GetReturnValue().Set(result);
}

NAN_METHOD(seedFromUint32) {
  if (info.Length() < 2) {
    return Nan::ThrowError("SeedFromUint32 takes at least two arguments");
  }

  if (info[0]->IsUint32() == false) {
    return Nan::ThrowTypeError("First argument should be an Uint32");
  }

  if (info[1]->IsArrayBuffer() == false) {
    return Nan::ThrowTypeError("Second argument should be an ArrayBuffer");
  }

  size_t offset = 0;

  if (info.Length() > 2) {
    if (info[2]->IsUndefined() == false) {
      if (info[2]->IsNumber() == false) {
        return Nan::ThrowTypeError("Second argument should be a number");
      }

      int64_t raw_offset = info[1]->IntegerValue();

      if (raw_offset < 0) {
        return Nan::ThrowRangeError("Offset cannot be negative");
      }

      offset = static_cast<size_t>(raw_offset);
    }
  }

  auto seed = info[0].As<v8::Uint32>()->Uint32Value();
  auto buffer = info[1].As<v8::ArrayBuffer>();

  if (buffer->ByteLength() < offset + 16) {
    return Nan::ThrowRangeError("Length out of range of buffer");
  }

  auto view = v8::Uint8Array::New(buffer, offset, 16);
  Nan::TypedArrayContents<uint32_t> typedData(view);

  auto raw = *typedData;

  // lcg_parkmiller
  // https://en.wikipedia.org/wiki/Lehmer_random_number_generator
  raw[0] = static_cast<uint32_t>((static_cast<uint64_t>(seed) * 48271UL) % 2147483647UL);
  raw[1] = static_cast<uint32_t>((static_cast<uint64_t>(raw[0]) * 48271UL) % 2147483647UL);
  raw[2] = static_cast<uint32_t>((static_cast<uint64_t>(raw[1]) * 48271UL) % 2147483647UL);
  raw[3] = static_cast<uint32_t>((static_cast<uint64_t>(raw[2]) * 48271UL) % 2147483647UL);
}

NAN_MODULE_INIT(Initialize) {
  XorShift128Plus::Init(target);
  NAN_EXPORT(target, seedFromUint32);
}

NODE_MODULE(xorshift128plus, Initialize)
