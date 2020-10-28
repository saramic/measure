# Measure

measure everything

* chicks.ipynb

## deploy YouTube Subscribers

```
# first time deploy
git subtree push --prefix youtube_subscribers/ heroku master

# subsequent time
git push --force heroku `git subtree split --prefix youtube_subscribers master`:master
```

