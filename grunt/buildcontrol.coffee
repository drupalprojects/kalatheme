module.exports =
  options:
    dir: 'build'
    commit: true
    push: true
    build:
      'Built %sourceName% from commit %sourceCommit% on branch %sourceBranch%'
  dev:
    options:
      remote: 'git@github.com:drupalprojects/kalatheme.git'
      branch: '<%= package.drupalversion =>'
  release:
    options:
      tag: '<%= package.drupalversion +"-"+ package.version =>'
