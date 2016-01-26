# XorShift128Plus

A seedable random number generator.

## Installation

```sh
npm install --save xorshift128plus
```

## Usage

```javascript
var XorShift128Plus = require('xorshift128plus')
var rng = XorShift128Plus.fromRandom()

rng.next() // random number
```

## API

### `new XorShift128Plus(buffer[, byteOffset])`

Creates a new RNG using the supplied ArrayBuffer and optional offset. Using the
RNG will modify the supplied buffer. The initial seed should be stored in the
buffer.

### `XorShift128Plus.fromRandom()`

Creates a new RNG with a psuedo-random seed chosen for you.

### `XorShift128Plus.fromHex(seed)`

Creates a new RNG with the seed specified as a string of 32 hexadecimal
characters. e.g. `'5d289450c888f99b5e5c1fd13509e39e'`.

### `XorShift128Plus.fromUint32(seed)`

Creates a new RNG with the seed specified as a unsigned 32bit integer. Note that
this seeding is suboptimal since it will only contain 32 bits of entropy instead
of 128 bits.

### `rng.next() -> Number`

Returns the next psuedo-random number between 0 (inclusivly) and 1 (exclusivly).
