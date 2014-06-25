# CONTRIBUTING.md

> We are more than happy to accept external contributions to the project in the form of feedback, bug reports and even better - pull requests.

## Reporting bugs and providing feedback

In order for us to help you please check that you've completed the following steps:

- Made sure you're on the latest version `$ drush dl kalatheme -y`
- Searched the issues queue to make sure that your bug hasn't already been reported.
- Included as much information about the bug as possible, including an error message, the expected behaviour, the observed behaviour, the version, etc.

[Submit your issue](https://github.com/drupalprojects/kalatheme/issues)



## Developers

The project will follow [github flow](https://guides.github.com/introduction/flow/index.html). Grunt will be used to build, test and bundle the kalatheme builds. All developement will be done off of master.

### Getting started

Project dependencies:

- [nodejs](http://nodejs.org/)
- Bower and Grunt
  - `$ npm install -g bower grunt-cli`
- Install local project packages
  - Inside the project repo run `$ npm install` and `$ bower install`


### Commit Messages

Use the [conventional commit message](https://github.com/ajoslin/conventional-changelog/blob/master/CONVENTIONS.md) style, we use this to generate the change log for the project.


## Submitting Pull Requests

- Open an issue first!
- After discussing, go ahead and fork this repo.
- Checkout a feature branch off of master.
- If possible, add a test for new features or a bug.
- Run existing tests `$ grunt`
- Clean up your commits,  using an [interactive rebase if needed.](http://git-scm.com/book/en/Git-Tools-Rewriting-History)



## Team members

- [Mike Pirog](https://github.com/pirog)
- [Steven Bassett](https://github.com/bassettsj)
- [Will Hastings](https://github.com/whastings)
- [Alec Reynolds](https://github.com/reynoldsalec)
- [Andrew Mallis](https://github.com/andrewmallis)
