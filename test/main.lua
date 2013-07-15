require 'test'
console.log('Hello World', 1, 2, nil, {}, 5.12)
console.info('Info')
console.warn('Warning')
console.error('Error')

local obj = {
    x = 2,
    y = 1,
    name = 'Foo'
}

console.log(obj)

console.log({1, 2, 3, 4, t = 'foo', sub = {10, 20}})

