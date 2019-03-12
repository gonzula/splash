# SPLASH : Simple Programming LAnguage for SHortcuts

![splash icon](https://raw.githubusercontent.com/gonzula/splash/master/imgs/RoundedIcon.png)
[![AppStore badge](https://linkmaker.itunes.apple.com/en-us/badge-lrg.svg?releaseDate=2019-03-11&kind=iossoftware&bubble=ios_apps)](https://itunes.apple.com/us/app/splash-programming-language/id1455793030?mt=8)
##### The first real programming language that compiles to Apple's `Shortcuts.app`

## What it is

Although the Shortcuts app is designed for non programmers/beginners, it's programming interface is similar to assembly in the meaning that very simple expressions need dozens of blocks.

To solve this problem, `SPLASH` is being developed as a programming language designed for non programmers/beginners that compiles directly to shortcuts.

![Example GIF](https://raw.githubusercontent.com/gonzula/splash/master/imgs/quadratic.gif)

## For what it's worth

Splash is meant to reduce substantially the manual labor, improve readability and maintainability of shortcuts. It's still under development but with a few fully working features. Between them:

* Complex mathematical expressions
* Flow control (ifs and elses)
* String interpolation (variables inside a string)

And those are some of the features in the backlog:

* Loops
* Functions declarations

## How it works

### The programming language

The best way to learn is with some examples

Here's an example splash program that given an age tells the person's stage of life.

[shortcut file](https://github.com/gonzula/splash/blob/master/examples/age.shortcut)

[video of the shortcut](https://github.com/gonzula/splash/blob/master/examples/age.mov)


``` BASH
age := AskNumber()  # The ':=' stores the right side expression
                    # on the left side variable

                    # And AskNumber() asks the user for a number input
                    # when the shortcut is running

if age < 12 {
    ShowResult("Child")
} else if age < 18 {  # Blocks of code are surrounded by '{' and '}'
    ShowResult("Teen")
} else if age < 60 {
    ShowResult("Adult")
} else {
    ShowResult("Elder")
}

# And comments are preceded by '#'
```

Here's an example with more advanced expressions that solves any quadratic expression in the form ax² + bx + c = 0

[shortcut file](https://github.com/gonzula/splash/blob/master/examples/quadratic.shortcut)

[video of the shortcut](https://github.com/gonzula/splash/blob/master/examples/quadratic.mov)

``` BASH
a := AskNumber()
b := AskNumber()
c := AskNumber()

delta := b^2 - 4 * a * c  # a^b is a to the b power

if a == 0 {
    x := -c/b

    answer := "x = {x}"  # This is a string interpolation
                         # It resolves to "x = (value of variable x)"
} else if delta == 0 {  # '==' tests for equality
    x := -b / (2 * a)

    answer := "x1 = x2 = {x}"
} else if delta > 0 {
    x1 := (-b + delta^(1/2))/(2 * a)
    x2 := (-b + -delta^(1/2))/(2 * a)

    answer := "x1 = {x1}\nx2 = {x2}"
} else {
    xr := -b / (2 * a)
    xi := (-delta)^(1/2) / (2 * a)
    nxi := -xi

    answer := "x1 = {xr} + {xi}i\nx2 = {xr} + {nxi}i"
}

ShowResult(answer)  # ShowResult shows an alert with the
                    # value passed inside the parenthesis
```

And a last example that tells if an year is a leap year:

[shortcut file](https://github.com/gonzula/splash/blob/master/examples/leap_year.shortcut)

[video of the shortcut](https://github.com/gonzula/splash/blob/master/examples/leap_year.mov)

``` BASH
year := AskNumber()

if year % 4 > 0 {  # The % symbol performs the modulo operation
    leap := 0
} else if year % 100 > 0 {  # And, diffently from shortcuts,
                            # you can have math expressions in
                            # the comparison
    leap := 1
} else if year % 400 > 0 {  # So this line checks if year is divisible by 400
    leap := 0
} else {
    leap := 1
}

if leap == 0 {
    type := "common"
} else {
    type := "leap"
}

ShowResult("{year} is a {type} year")
```

## How to get started

You can use this language on your iOS device by [downloading the app from the App Store](https://itunes.apple.com/us/app/splash-programming-language/id1455793030?mt=8) or cloning this repo and compiling it on your Xcode. (You will need an Apple Developer account)

Or you can compile the compiler on your computer. It's pure C code, without any dependencies, so it works on any operating system.

## Installing the app via Xcode

You will need to have installed `bison` installed.
```
brew install bison
ln -s /usr/local/opt/bison/bin/bison /usr/local/bin/bison
```
And then it's just a matter of building the Xcode project

## Compiling the Compiler

### The easy way

Download the compiled version from the latest release

[linux version](https://github.com/gonzula/splash/releases/download/v0.1.1/splash.linux)

[macOS version](https://github.com/gonzula/splash/releases/download/v0.1.1/splash.macOS)

and run

``` BASH
chmod +x splash
```


### Compiling from source

You can compile the splash compiler from source, cloning this repo and running `make`. You will need `bison`, `flex` and a C compiler

On macOS you can install the dependencies with homebrew:

```
brew install bison
ln -s /usr/local/opt/bison/bin/bison /usr/local/bin/bison
```

On ubuntu:

```
sudo apt install bison flex gcc
```

## How to run

On a terminal window located at the folder you installed `splash`

```
./splash input_file output_file
```

The splash compiler adds a `.shortcut` extension to output_file that is required by the shortcuts app.

Also, the name of that output file is the display name of your shortcut.

## How to import the shortcuts

The easiest way is to airdrop the `.shortcut` file to your device.
