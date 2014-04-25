module.exports =

  default: [
    'clean:dist'
    'js'
    'tests'
    'css'
    'img'
  ]
  js: [
    'clean:temp'
    'coffeelint:theme'
    'coffee:theme'
    'concat:js'
    'uglify:js'
  ]
  tests: [
    'coffeelint:tests'
    'coffee:specs'
    'jasmine:specs'
  ]
  img: [
    'imagemin:theme'
  ]
  css: [
    'sass:dist'
    'autoprefixer:theme'
    'cssmin:theme'
  ]
  develop: [
    'connect:tests'
    'watch'
  ]
