#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
from collections import namedtuple
import csv
import urllib.request
import sys


csv_url = 'http://id.loc.gov/vocabulary/iso639-5.tsv'


def fetch_url(url: str) -> str:
    r = urllib.request.Request(url=url, headers={
        # 'User-Agent': 'Mozilla/5.0 (X11; Fedora; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome Safari/537.36',
        # 'User-Agent': 'Python not Mozilla/5.0 or Gecko or Chrome or AppleWebKit',
    })
    with urllib.request.urlopen(r) as f:
        data = f.read().decode()
        return data


def main_fetch(ns: argparse.Namespace):
    url = ns.url if ns.url else csv_url
    output = open(ns.output, 'w') if ns.output else sys.stdout
    data = fetch_url(url)
    output.write(data)


CodeName = namedtuple('CodeName', ('code', 'name', ))


def main_parse(ns: argparse.Namespace):
    csvreader = csv.reader(open(ns.input, 'r'), delimiter='\t')
    iter_csv = iter(csvreader)
    title_row = next(iter_csv)
    assert len(title_row) == 4, 'iso-639-5 table must contain 4 columns'
    uri_index = title_row.index('URI')
    code_index = title_row.index('code')
    eng_index = title_row.index('Label (English)')
    fre_index = title_row.index('Label (French)')
    all_data = []
    for row in iter_csv:
        code = row[code_index]
        name = row[eng_index]
        item = CodeName(code=code, name=name)
        all_data.append(item)
    import pprint
    pprint.pprint(all_data)
    return all_data


def main():
    parser = argparse.ArgumentParser('Generate iso639-3 language list')

    subparsers = parser.add_subparsers()

    fetch_parser = subparsers.add_parser('fetch', description='Fetch resource')
    fetch_parser.add_argument('-o', metavar='FILE', dest='output', default=None,
                              help='where to write fetched data to')
    fetch_parser.add_argument('-u', metavar='URL', dest='url', default=None,
                              help='iso639-3 url')
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
