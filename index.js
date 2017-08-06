let nativeModule = null;
let tried = false;

function initializeModule() {
  if (!tried) {
    tried = true;
    try {
      nativeModule = require('./build/Release/process-info');
    } catch (err) {
      console.error(err);
    }
  }
}

exports.getProcessInfo = function (pids) {
  initializeModule();
  if (!nativeModule) {
    return null;
  }

  let isArray = Array.isArray(pids);

  if (!isArray) {
    pids = [pids];
  }

  let result;
  try {
    result = nativeModule.getProcessInfo(pids);
  } catch (err) {
    console.error(err);
    return null;
  }

  if (!isArray) {
    return (result.length != 0 ? result[0] : null);
  } else {
    return result;
  }
};