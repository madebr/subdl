#!/usr/bin/env python3

import argparse
import csv
import datapackage
import sys


datasource_url = 'https://datahub.io/core/country-codes/datapackage.json'


def main_fetch(ns: argparse.Namespace):
    url = ns.url if ns.url else datasource_url
    output = open(ns.output, 'w') if ns.output else sys.stdout
    package = datapackage.Package(url)

    resource = package.get_resource('country-codes')
    data = resource.read()

    csv_writer = csv.writer(output)
    csv_writer.writerow(resource.headers)
    csv_writer.writerows(data)


def main_parse(ns: argparse.Namespace):
    input = open(ns.input, 'r') if ns.input else sys.stdint
    csv_reader = csv.reader(input)
    csv_row_iter = iter(csv_reader)
    headers = next(csv_row_iter)
    data = list(csv_row_iter)


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
