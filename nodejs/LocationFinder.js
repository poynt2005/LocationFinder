var path = require("path");
var fs = require("fs");

/** Custom Error: RuntimeError */
function RuntimeError(message) {
  this.name = "RuntimeError";
  this.message = message || "Error running the current script";
  this.stack = new Error().stack;
}

var _f = function () {};
_f.prototype = Error.prototype;
RuntimeError.prototype = new _f();
RuntimeError.prototype.constructor = RuntimeError;
/** */

if (
  !fs.existsSync("LocationFinder.dll") ||
  !fs.existsSync("LocationFinder.node")
) {
  throw new RuntimeError("Dependency library file not found");
}

var nativeBinding = require("./LocationFinder.node");

var findType = Object.defineProperties(
  {},
  {
    VISUAL_STUDIO: {
      value: "visual_studio",
      writable: false,
      enumerable: true,
    },
    PYTHON: { value: "python", writable: false, enumerable: true },
    NODE_JS: { value: "node_js", writable: false, enumerable: true },
  }
);

/**
 * Function: finder, check the software(MSVC, python, node.js) install path
 * @param {String} inputFindType - The software install path you want to check, you can use the enum type "findType"
 * @returns {String[]} - The install path stored in array
 */
var finder = function (inputFindType) {
  return nativeBinding.Finder(inputFindType);
};

module.exports = {
  findType,
  finder,
};
