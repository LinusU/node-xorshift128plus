/* eslint-env mocha */

'use strict'

const assert = require('assert')
const native = require('../build/Release/xorshift128plus.node')

const XorShift128Plus = native.XorShift128Plus

describe('Native module', function () {
  it('does bounds checking', function () {
    const smallBuffer = new ArrayBuffer(10)
    const perfectBuffer = new ArrayBuffer(16)
    const largeBuffer = new ArrayBuffer(30)

    assert.throws(
      () => new XorShift128Plus(smallBuffer),
      RangeError
    )

    assert.throws(
      () => new XorShift128Plus(perfectBuffer, 1),
      RangeError
    )

    assert.throws(
      () => new XorShift128Plus(largeBuffer, 16),
      RangeError
    )

    assert.throws(
      () => new XorShift128Plus(largeBuffer, 40),
      RangeError
    )

    assert.throws(
      () => new XorShift128Plus(largeBuffer, -2),
      RangeError
    )
  })

  it('works with 0', function () {
    const buffer = new ArrayBuffer(16)
    const rng = new XorShift128Plus(buffer)

    assert.equal(rng.next(), 0)
    assert.equal(rng.next(), 0)
    assert.equal(rng.next(), 0)
    assert.equal(rng.next(), 0)
    assert.equal(rng.next(), 0)
  })

  it('works with seed', function () {
    const buffer = new ArrayBuffer(16)
    const rng = new XorShift128Plus(buffer)
    const view = new Uint32Array(buffer)

    native.seedFromUint32(4293262078, buffer)

    assert.equal(view[0], 1439380697)
    assert.equal(view[1], 659709849)
    assert.equal(view[2], 1966603363)
    assert.equal(view[3], 396319738)

    assert.equal(rng.next(), 0.4335893835472515)
    assert.equal(rng.next(), 0.6067907909036327)
    assert.equal(rng.next(), 0.046905965279849804)
    assert.equal(rng.next(), 0.480991995797152)
    assert.equal(rng.next(), 0.6796126170804464)
  })
})
