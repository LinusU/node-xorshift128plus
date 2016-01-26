/* eslint-env mocha */

'use strict'

const assert = require('assert')
const XorShift128Plus = require('../')

describe('API', function () {
  it('#fromRandom', function () {
    let rng = XorShift128Plus.fromRandom()

    let rand0 = rng.next()
    let rand1 = rng.next()

    assert(rand0 + rand1 > 0)
    assert(rand0 >= 0 && rand0 < 1)
    assert(rand1 >= 0 && rand1 < 1)
  })

  it('#fromHex', function () {
    let rng = XorShift128Plus.fromHex('5d289450c888f99b5e5c1fd13509e39e')

    assert.equal(rng.next(), 0.35873106038177727)
    assert.equal(rng.next(), 0.7433543130711686)
    assert.equal(rng.next(), 0.6325316214071923)
    assert.equal(rng.next(), 0.708663591569944)
    assert.equal(rng.next(), 0.8974382234842848)
  })

  it('#fromUint32', function () {
    let rng = XorShift128Plus.fromUint32(4293262078)

    assert.equal(rng.next(), 0.4335893835472515)
    assert.equal(rng.next(), 0.6067907909036327)
    assert.equal(rng.next(), 0.046905965279849804)
    assert.equal(rng.next(), 0.480991995797152)
    assert.equal(rng.next(), 0.6796126170804464)
  })
})
