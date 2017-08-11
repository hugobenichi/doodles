let test = (x) => {
  if (x) {
    console.log('was true');
  }
};

// expected null values
test(); // nothing !
test(undefined);
test(null);

// booleans, as expected
test(true);
test(false);

// numbers
test(1); // 'was true'
test(0); // nothing !

// strings
test(''); // nothing !
test('something'); // was true

// arrays, and objects
test([]); // was true
test([1]); // was true
test({}); // was true

test()
