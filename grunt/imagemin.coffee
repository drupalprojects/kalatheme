module.exports =
  png:{
    files: [
      {
        expand: true,
        cwd: 'img-src'
        src: ['**/*.png']
        dest: 'dist/img'
      }
    ]
    options:
      progressive: false
  }
  gif:{
    files: [
      {
        expand: true,
        cwd: 'img-src'
        src: ['**/*.gif']
        dest: 'dist/img'
      }
    ]
    options:
      progressive: false
  }
  jpg:{
    files: [
      {
        expand: true,
        cwd: 'img-src'
        src: ['**/*.jpg']
        dest: 'dist/img'
      }
    ]
    options:
      progressive: true
  }
  icons:
    files: {
      'dist/img/kalatheme-sprite.png' : 'dist/img/kalatheme-sprite.png'
    }
    options:
      progressive: false
