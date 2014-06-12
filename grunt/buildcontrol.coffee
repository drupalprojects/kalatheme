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
      branch: '<%= package.drupalversion =>'
  release:
    options:
      branch: '<%= package.drupalversion =>'
      tag: '<%= package.drupalversion +"-"+ package.version =>'
