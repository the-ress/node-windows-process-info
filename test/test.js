var index = require('../index')

let now = new Date();
let nodeReleased = new Date(2009, 5, 27);

console.log('Testing getProcessInfo(process.pid)')
var result = index.getProcessInfo(process.pid)
console.assert(result, 'Should return result')
console.assert(!Array.isArray(result), 'Result should not be an array')
console.assert(result.name === 'node.exe', 'Process name should be "node.exe"')
console.assert(result.path, 'Path should not be empty')
console.assert(result.processId === process.pid, 'Process PID should be same as `process.pid`')
console.assert(result.parentId, 'Parent PID should not be empty')
console.assert(result.startTime < now, 'Start time should not be in the future')
console.assert(nodeReleased < result.startTime, 'Start time should not too far in the past')


console.log('Testing getProcessInfo(-1)')
var result = index.getProcessInfo(-1)
console.assert(result === null, 'Should return null')


console.log('Testing getProcessInfo([process.pid, 4, -1])')
var result = index.getProcessInfo([process.pid, 4, -1])
console.assert(Array.isArray(result), 'Should return an array')
console.assert(result.length === 2, 'Should return 2 processes')
result = result.sort((a, b) => a.processId - b.processId)

console.assert(result[0].name === 'System', 'Process name should be "System"')

console.assert(result[1].name === 'node.exe', 'Process name should be "node.exe"')
console.assert(result[1].path, 'Path should not be empty')
console.assert(result[1].parentId, 'Parent PID should not be empty')
console.assert(result[1].startTime < now, 'Start time should not be in the future')
console.assert(nodeReleased < result[1].startTime, 'Start time should not too far in the past')

console.log('All tests passed')