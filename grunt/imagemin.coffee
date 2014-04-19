module.exports =
  theme:
    files: [
      {
        expand: true,
        cwd: 'img-src'
        src: ['**/*.{png,jpg,gif}']
        dest: 'dist/img'
      }
    ]
