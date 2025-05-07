// In test.js
const calculator = require('../script');  // Import all functions as a single object

var chai = require('chai');
var assert = require('assert');

describe('cos function', function() {
  it('should compute the cosine of the input value', function() {
    // Simulate the form object with a display value
    let form = {display : { value: "0" }};
    calculator.cos(form);
    assert.equal(form.display.value, 90);
  });
});

describe('sine function', function() {
    it('should compute the sine of the input value', function() {
      // Simulate the form object with a display value
      let form = {display : { value: "0" }};
      calculator.sin(form);
      assert.equal(form.display.value, 0);
    });
});

describe('tan function', function() {
    it('should compute the tan of the input value', function() {
      // Simulate the form object with a display value
      let form = {display : { value: "0" }};
      calculator.tan(form);
      assert.equal(form.display.value, 0);
    });
});

describe('sqrt function', function() {
    it('should compute the square root of the input value', function() {
      // Simulate the form object with a display value
      let form = {display : { value: "-9" }};  // Square root of 9 is 3
      calculator.sqrt(form);
      assert.equal(form.display.value, NaN);
    });
});

// describe('ln function', function() {
//     it('should return NaN for values less than or equal to 0', function() {
//       // Simulate the form object with a display value
//       let form = {display : { value: "0" }};  // ln(0) is undefined (NaN)
//       calculator.ln(form);
//       assertIsNone(form.display.value);  // Check if the result is NaN
//     });
//   });
  


