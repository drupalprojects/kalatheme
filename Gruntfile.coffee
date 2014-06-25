###
@file Gruntfile.coffee
Provides developer tasks to build assets for the theme.
see `./grunt` for tasks
###


module.exports = (grunt) ->
  # Autoloads tasks and config
  require('load-grunt-config')( grunt, {
    data: {
      pkg: grunt.file.readJSON('package.json')
    }
  })

  grunt.registerTask('fontawesome'
    , 'builds the font awesome config for kalatheme'
    , ['curl-dir:fontawesomeicons']
    , () ->

      fontAwesome = grunt.file.readYAML('./temp/icons.yml')
      grunt.file.write('./includes/icons/fontawesome.json', JSON.stringify(fontAwesome))
  )
