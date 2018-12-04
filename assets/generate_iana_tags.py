#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
from urllib.request import urlopen
import sys

language_subtag_registry_url = 'https://www.iana.org/assignments/language-subtag-registry/language-subtag-registry'


def fetch_url(url: str) -> str:
    with urlopen(url) as f:
        return f.read().decode('utf-8')


def main_fetch(ns: argparse.Namespace):
    url = ns.url if ns.url else language_subtag_registry_url
    output = open(ns.output, 'w') if ns.output else sys.stdout
    data = fetch_url(url)
    output.write(data)


def main_parse(ns: argparse.Namespace):
    input = open(ns.input, 'r') if ns.input else sys.stdin
    txt = input.read()
    data = parse_source(txt)
    print(data)

    import pprint
    # pprint.pprint(group_by(data, 'Type'))
    g = group_by(data, 'Type')
    # pprint.pprint(g['language'])
    g['language'].sort(key=lambda l: l['Description'][0])

    for lang in g['language']:
        print('{{ {}, {} }}'.format(lang['Subtag'], lang['Description']))


def parse_source(src: str):
    items = []
    current = {}
    last_key = None
    for line in src.splitlines(keepends=False):
        if line == '%%':
            items.append(current)
            current = {}
            last_key = None
            continue
        key_value = line.split(': ', maxsplit=1)
        if len(key_value) == 2:
            [key, value] = key_value
            if key == 'Description':
                current.setdefault(key, []).append(value)
            else:
                current[key] = value
            last_key = key_value[0]
        else:
            current[last_key] += ' ' + line.strip()
            # print('UNKNOWN: {} (prev={})'.format(line, current))
    if current:
        items.append(current)
    return items


def group_by(data, key):
    result = {}
    for item in data:
        try:
            t = item[key]
        except KeyError:
            continue
        result.setdefault(t, []).append(item)
    return result


def main():
    parser = argparse.ArgumentParser('Generate language list from iana language subtag registry')

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

    ns = parser.parse_args()
    if 'func' not in ns:
        parser.error('Need function')
    ns.func(ns)


if __name__ == '__main__':
    main()
