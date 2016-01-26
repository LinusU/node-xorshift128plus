var native = require('./build/Release/xorshift128plus.node')

var XorShift128Plus = native.XorShift128Plus

XorShift128Plus.fromRandom = function fromRandom () {
  var buffer = new ArrayBuffer(16)
  var view = new Uint32Array(buffer)

  view[0] = Math.floor(Math.random() * Math.pow(2, 32))
  view[1] = Math.floor(Math.random() * Math.pow(2, 32))
  view[2] = Math.floor(Math.random() * Math.pow(2, 32))
  view[3] = Math.floor(Math.random() * Math.pow(2, 32))

  if (view[0] + view[1] + view[2] + view[3] === 0) {
    return XorShift128Plus.fromRandom()
  }

  return new XorShift128Plus(buffer)
}

XorShift128Plus.fromHex = function fromHex (seed) {
  if (typeof seed !== 'string') throw new TypeError('seed must be a string')
  if (seed.length !== 32) throw new TypeError('seed must be of 32 characters')

  var buffer = new ArrayBuffer(16)
  var view = new Buffer(buffer)

  view.write(seed, 'hex')

  return new XorShift128Plus(buffer)
}

XorShift128Plus.fromUint32 = function fromUint32 (seed) {
  var buffer = new ArrayBuffer(16)

  native.seedFromUint32(seed, buffer)

  return new XorShift128Plus(buffer)
}

module.exports = XorShift128Plus
