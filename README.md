# windows-process-info

Gets process name, parent ID, executable path and start time for PID

## Installing

```sh
npm install windows-process-info
```

## Using

```javascript
var getProcessInfo = require('process-info').getProcessInfo
console.log(getProcessInfo(process.pid))
console.log(getProcessInfo([process.pid, anotherPid]))
```

## Supported OSes
 * windows

## Developing
 * `npm install -g node-gyp`
 * `node-gyp configure`
 * `node-gyp build`
 * `npm test`

## License
[MIT](LICENSE)
