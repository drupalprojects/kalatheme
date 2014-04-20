##* Grunt Contrib Watch
module.exports =
  tests:
    files: ['test/spec/**/*.coffee']
    tasks: ['tests']
  styles:
    files: ['sass/**/*.scss']
    tasks: ['csscss:theme','sass:develop']
  coffee:
    files: ['src/**/*.coffee']
    tasks: [
      'clean:temp'
      'coffeelint:theme'
      'coffee:theme'
      'concat:js'
    ]
