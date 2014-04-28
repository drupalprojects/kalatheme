module.exports =

  default: [
    'clean:dist'
    'img'
    'js'
    'tests'
    'css'
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
    'sprite:icons'
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
