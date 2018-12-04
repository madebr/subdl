#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
from collections import namedtuple
import urllib.request
from bs4 import BeautifulSoup
import sys

loc_url = 'http://www.loc.gov/standards/iso639-2/php/code_list.php'

LangItem = namedtuple('LangItem', ('iso639_1', 'iso639_2', 'name'))


def fetch_url(url: str) -> str:
    r = urllib.request.Request(url=url, headers={
        'User-Agent': 'Python',
    })
    with urllib.request.urlopen(r) as f:
        data = f.read()
        return data.decode('cp1252')


def main_fetch(ns: argparse.Namespace):
    src = fetch_url(ns.url if ns.url else loc_url)
    # src = src.replace('&nbsp;', '')
    output = open(ns.output, 'w') if ns.output else sys.stdout
    output.write(src)


def main_parse(ns: argparse.Namespace):
    src = open(ns.input, 'r').read()
    output = open(ns.output, 'w') if ns.output else sys.stdout

    # needs html5lib: python -m pip install --user html5lib
    bs = BeautifulSoup(src, 'html5lib')
    table = bs.table.table
    title_row = table.tr
    titles_substr = ['639-2', '639-1', 'English name']
    iso639_1_index = titles_substr.index('639-1')
    iso639_2_index = titles_substr.index('639-2')
    name_index = titles_substr.index('English name')
    columns = [None] * len(titles_substr)
    html_header_columns = title_row.findChildren('th')
    for column_i, column in enumerate(html_header_columns):
        for title_i, title_str in enumerate(titles_substr):
            if title_str in column.text:
                columns[title_i] = column_i
    if not all(col is not None for col in columns):
        raise RuntimeError('Columns not found!')

    all_langs = {}
    special_langs = {}

    for row in title_row.next_siblings:
        children = row.findChildren('td')
        iso639_1 = children[iso639_1_index].string.strip()
        iso639_1 = iso639_1 if iso639_1 else None
        iso639_2 = [s.rstrip(' (B)').rstrip(' (T)').strip() for s in children[iso639_2_index].strings]
        if not iso639_2:
            raise RuntimeError('Illegal data row')

        name = children[name_index].string.strip()
        name = name.split('; ')
        data = LangItem(iso639_1=iso639_1, iso639_2=iso639_2, name=name)

        key = iso639_2[0]
        if key in ('mis', 'mul', 'qaa-qtz', 'und', 'zxx'):
            assert key not in special_langs
            special_langs[key] = data
        else:
            if key in all_langs:
                assert data == all_langs[key]
            else:
                all_langs[key] = data

    # FIXME: print to output
    import pprint
    pprint.pprint(all_langs)
    pprint.pprint(special_langs)
    for lang in all_langs.values():
        if lang.iso639_1:
            assert len(lang.iso639_1) == 2, '{} must be of length 2'.format(data.iso639_1)
        for lang_xxx in lang.iso639_2:
            assert len(lang_xxx) == 3, '{} must be of length 3'.format(lang_xxx)


def main():
    parser = argparse.ArgumentParser('Generate iso639-1/2 language list')

    subparsers = parser.add_subparsers()

    fetch_parser = subparsers.add_parser('fetch', description='Fetch resource')
    fetch_parser.add_argument('-o', metavar='FILE', dest='output', default=None,
                         help='where to write fetched data to')
    fetch_parser.add_argument('-u', metavar='URL', dest='url', default=None,
                         help='iso639-1/2 url')
    fetch_parser.set_defaults(func=main_fetch)

    parse_parser = subparsers.add_parser('parse', description='Parse resource')
    parse_parser.add_argument('-i', metavar='FILE', dest='input', default=None,
                              help='iso639 file source', required=True)
    parse_parser.add_argument('-o', metavar='FILE', dest='output', default=None,
                              help='parsed output')
    parse_parser.set_defaults(func=main_parse)

    ns = parser.parse_args()
    if 'func' not in ns:
        parser.error('Need function')
    ns.func(ns)


if __name__ == '__main__':
    main()
