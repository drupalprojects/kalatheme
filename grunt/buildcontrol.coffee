module.exports =
  options:
    dir: 'build'
    commit: true
    push: true
    remote: 'git@github.com:drupalprojects/kalatheme.git'
    build:
      'Built %sourceName% from commit %sourceCommit% on branch %sourceBranch%'
  dev:
    options:
      branch: '7.x-4.x'
  release:
    options:
      branch: '<%= package.drupalversion =>'
      tag: '<%= package.drupalversion +"-"+ package.version =>'
