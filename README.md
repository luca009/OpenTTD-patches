## A few small patches to OpenTTD, based on JGR's Patch Pack
This repo gets updated to the latest version of JGRPP occasionally. Learn more about JGR's Patch Pack and OpenTTD [here](https://github.com/JGRennison/OpenTTD-patches/blob/jgrpp/README.md).

My custom patches currently include:
- More options regarding running costs of vehicles while stationary (as configurable in the settings)
  - Allows up to 16x less running costs when stationary
  - Allows disabling running costs when stationary all together (useful for scheduled dispatching)
- A more balanced way to calculate station ratings
  - A vehicle's age is now less important in the calculation
  - Passengers are more patient now and bulk goods (like coal or iron ore) are even more patient now, ideal for occasional pickups.
