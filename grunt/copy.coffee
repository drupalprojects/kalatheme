module.exports =
  buildfiles:
    files: [
      {
        src: [
          'templates/**/*'
          'views/**/*'
          'styles/**/*'
          'dist/**/*'
          'includes/**/*'
          'favicon.ico'
          'screenshot.png'
          'logo.png'
          'readme.md'
          'template.php'
          'theme-settings.php'
          './kalatheme.*'
        ]
        dest: 'build/'
      },{
        src: 'build-gitignore'
        dest: 'build/.gitignore'
      }
    ]
