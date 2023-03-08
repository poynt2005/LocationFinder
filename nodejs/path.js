var path = require("path");

var LocationFiner = path
  .resolve("../csdll/LocationFinder/LocationFinder")
  .replace(/\\/gm, "\\\\");

var LocationFinerLib = path
  .resolve("../csdll/LocationFinder/x64/Debug/LocationFinder.lib")
  .replace(/\\/gm, "\\\\");

module.exports = {
  LocationFiner: LocationFiner,
  LocationFinerLib: LocationFinerLib,
};
