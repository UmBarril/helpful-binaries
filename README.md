
# Helpful Binaries

Some helpful small C programs that I've written and I use daily.

I use them in alongside my [Dotfiles](https://github.com/UmBarril/dotfiles) scripts.

Maybe you will find them useful too.

# Why C?

First I started writing them in Bash, but then I noticed that I needed them to work with SUID and SGID permissions.

And as you may know, Bash scripts can't have SUID or SGID permissions (in fact, any scripted language can't).

So I had to write them in a compiled language, and C was just one of the options.

Someday I will rewrite them in Zig or Rust maybe, but for now, C is enough.

# How to compile

To compile any of the binaries, you just need to run the following command:

```bash
make <binary>
```

