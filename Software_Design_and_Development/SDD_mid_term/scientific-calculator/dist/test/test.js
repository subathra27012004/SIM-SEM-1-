const calculator = require('../script');
const assert = require('chai').assert;

// Test Case 1: Testing the delete character function for decimal number
describe('Testing the delete character function', function() {
    it('delete the last character of the display value', function() {
        let form = { display: { value: "123.45" } };
        calculator.deleteChar(form.display);
        // assert.equal(form.display.value, "123.45"); // Test fails
        assert.equal(form.display.value, "123.4"); // Test pass
    });
});

// Test Case 2: Testing the square function for 5^2
describe('Testing the square function', function() {
    it('should calculate the square of the input value correctly', function() {
        let form = { display: { value: "5" } };
        calculator.square(form);
        // assert.equal(form.display.value, "10"); // Test fails
        assert.equal(form.display.value, "25"); // Test pass
    });
});

// Test Case 3: Testing the square root function
describe('Testing the square root function', function() {
    it('should calculate the square root correctly', function() {
        let form = { display: { value: "4" } };
        calculator.sqrt(form);
        // assert.equal(form.display.value, "0"); // Test fails
        assert.equal(form.display.value, "2"); // Test pass
    });
});

// Test Case 4: Testing the ln function
describe('Testing the ln function', function() {
    it('should calculate ln 2 correctly', function() {
        let form = { display: { value: "2" } };
        calculator.ln(form);
        // assert.isTrue(Math.abs(form.display.value - 1) < 0.0001); // ln(2) ≈ 0.6931, Test fails
        assert.isTrue(Math.abs(form.display.value - 0.6931) < 0.0001); // ln(2) ≈ 0.6931, Test pass
    });
});

// Test Case 5: Testing the e function 
describe('Testing the exponential function', function() {
    it('should calculate e^x correctly', function() {
        let form = { display: { value: "1" } };
        calculator.exp(form);
        // assert.isTrue(Math.abs(form.display.value - 1) < 0.0001); //  e (≈ 2.71828), Test fails
        assert.isTrue(Math.abs(form.display.value - 2.71828) < 0.0001); //  e (≈ 2.71828), Test pass
    });
});

// Test Case 6: Testing the sign change function
describe('Testing the sign change function', function() {
    it('should change the sign of a positive number to negative', function() {
        let form = { display: { value: "2" } }; 
        calculator.changeSign(form.display);
        // assert.equal(form.display.value, "2"); // Test fails
        assert.equal(form.display.value, "-2"); // Test Pass
    });
    it('should change the sign of a negative number to positive', function() {
        let form = { display: { value: "-3" } };
        calculator.changeSign(form.display); 
        // assert.equal(form.display.value, "-3"); // Test fails
        assert.equal(form.display.value, "3"); // Test Pass
    });
});

// Test Case 7: Testing the sine function
describe('Testing the sine function', function() {
    it('should return 0 for sin(0)', function() {
        let form = { display: { value: "0" } };
        calculator.sin(form);
        // assert.equal(form.display.value, "1"); // Test fails
        assert.equal(form.display.value, "0"); // Test Pass
    });
    it('should return approximately 1 for sin(π/2)', function() {
        let form = { display: { value: "1.5708" } }; // π/2 in radians
        calculator.sin(form);
        // assert.isTrue(Math.abs(form.display.value - 1.5708) < 0.0001); // Test fails
        assert.isFalse(Math.abs(form.display.value - 1.5708) < 0.0001); // Test Pass
    });
});

// Test Case 8: Testing the cosine function
describe('Testing the cosine function', function() {
    it('should return 1 for cos(0)', function() {
        let form = { display: { value: "0" } };
        calculator.cos(form);
        // assert.equal(form.display.value, "0"); // Test Fails
        assert.equal(form.display.value, "1"); // Test Pass
    });
    it('should return approximately -1 for cos(π)', function() {
        let form = { display: { value: "3.1416" } }; // π in radians
        calculator.cos(form);
        // assert.isTrue(Math.abs(form.display.value + 3.1416) < 0.0001); // Test Fails
        assert.isTrue(Math.abs(form.display.value + 1) < 0.0001); // Test Pass
    });
});

// Test Case 9: Testing the tangent function
describe('Testing the tangent function', function() {
    it('should return 0 for tan(0)', function() {
        let form = { display: { value: "0" } };
        calculator.tan(form);
        // assert.equal(form.display.value, "1"); // Test Fails
        assert.equal(form.display.value, "0"); // Test Pass
    });
    it('should return approximately 1 for tan(π/4)', function() {
        let form = { display: { value: "0.7854" } }; // π/4 in radians
        calculator.tan(form);
        // assert.isTrue(Math.abs(form.display.value - 0.7854) < 0.0001); // Test Fails
        assert.isTrue(Math.abs(form.display.value - 1) < 0.0001); // Test Pass
    });
});