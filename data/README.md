Training data and the ready to use models can be downloaded from http://cental.fltr.ucl.ac.be/team/panchenko/stc/data.tgz. 
Download this archive and extract to this directory:

```
wget http://cental.fltr.ucl.ac.be/team/panchenko/stc/data.tgz && tar xzf data.tgz && rm -f data.tgz
```

The archive contains several models:

- **gallery** -- The *Gallery* dataset, composed of titles of pornographic galleries (positives) and wikipedia articles (negatives). Here the model was trained on 90% of the data (train.xml) and can be tested on 10% of the rest data (valid.xml)

- **gallery-full** -- The *Gallery* dataset. Here the model was train on the entire dataset (train.xml contain 100% of the dataset).

- **tdt** -- The *PirateBay Title-Description-Tag* dataset, composed of file names of PirateBay torrent tracker. Positives are file names under the 'Porn' category. Negatives are file names of all other categories e.g. 'Games', 'Music', etc.  Here the model was trained on 90% of the data (train.xml) and can be tested on 10% of the rest data (valid.xml)

- **tdt-full** -- The *PirateBay Title-Description-Tag*. Here the model was train on the entire dataset (train.xml contain 100% of the dataset).

- **tt** -- The *PirateBay Title-Tag* dataset, composed of file names of PirateBay torrent tracker. Positives are file names under the 'Porn' category. Negatives are file names of all other categories e.g. 'Games', 'Music', etc.  Here the model was trained on 90% of the data (train.xml) and can be tested on 10% of the rest data (valid.xml)

- **tt-full** -- The *PirateBay Title-Tag*. Here the model was train on the entire dataset (train.xml contain 100% of the dataset).

- **relations.csv** -- Set of semantic relations used by all the models.

- **relations-short.csv** -- An alternative small scale set of semantic relations.

