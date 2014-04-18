module.exports =

  default: [
    'clean:dist'
    'clean:temp'
    'coffeelint'
    'coffee'
    'concat'
    'uglify'
  ]
