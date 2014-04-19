module.exports =

  default: [
    'clean:dist'
    'clean:temp'
    'coffeelint:theme'
    'coffee:theme'
    'concat:js'
    'uglify:js'
    'tests'
    'sass:theme'
    'autoprefixer:theme'
  ]
  tests: [
    'coffeelint:tests'
    'coffee:specs'
    'jasmine:specs'
  ]
