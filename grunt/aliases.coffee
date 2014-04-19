module.exports =

  default: [
    'clean:dist'
    'clean:temp'
    'coffeelint:theme'
    'coffee:theme'
    'concat:js'
    'uglify:js'
    'tests'
  ]
  tests: [
    'coffeelint:tests'
    'coffee:specs'
    'jasmine:specs'
  ]
