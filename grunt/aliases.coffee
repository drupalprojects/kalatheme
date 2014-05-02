module.exports =

  default: [
    'clean:dist'
    'img'
    'js'
    'css'
    'curl-dir:drupaljs'
    'tests'
  ]
  js: [
    'clean:tempjs'
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
    # 'csscss:theme'
    # 'csslint:theme'
    'cssmin:theme'
  ]
  develop: [
    'connect:tests'
    'watch'
  ]
