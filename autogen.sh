#!/bin/sh

AUTORECONF=`which autoreconf`

if [[ -z $AUTORECONF ]]; then
    echo "No autoreconf found, please install it."

    exit 1
fi

autoreconf --force --install || exit $
