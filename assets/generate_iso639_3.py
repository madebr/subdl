#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
from collections import namedtuple
import csv
import io
from pathlib import Path
import re
import urllib.error
import urllib.request
import zipfile


url_iso639_3 = 'http://iso639-3.sil.org/sites/iso639-3/files/downloads/iso-639-3_Code_Tables_20180914.zip'


def fetch_url(url: str) -> str:
    r = urllib.request.Request(url=url, headers={
        'User-Agent': 'Mozilla/5.0 (X11; Fedora; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome Safari/537.36',
    })
    with urllib.request.urlopen(r) as f:
        data = f.read()
        return data


def main_fetch(ns: argparse.Namespace):
    data = fetch_url(ns.url if ns.url else url_iso639_3)
    open(ns.output, 'wb').write(data)


def main_parse(ns: argparse.Namespace):
    zip = zipfile.ZipFile(ns.input)
    dirs = list(filter(lambda info: info.is_dir(), zip.infolist()))
    assert len(dirs) == 1, 'Expecting 1 directory'
    dirpath = Path(dirs[0].filename)
    files = {
        'macrolanguages': None,
        'index': None,
        'Name_Index': None,
        'Retirements': None,
    }
    for zipinfo in zip.infolist():
        if zipinfo.is_dir():
            continue
        filename = str(Path(zipinfo.filename).relative_to(dirpath))
        regex = re.compile('iso-639-3-?_?([a-zA-Z_]*)[0-9]+.tab', re.VERBOSE | re.X)
        m = regex.match(filename)  # re.search('iso-639-3(-[a-zA-Z_]+)_[0-9]+.tab', filename, re.VERBOSE)
        if not m:
            raise RuntimeError('Illegal filename: {}'.format(filename))
        name = m.groups(1)[0].strip('_')
        if name == '':
            name = 'index'
        assert name in files
        files[name] = zip.read(zipinfo.filename).decode()
    all_data = parse_index(files['index'])
    import pprint
    pprint.pprint(all_data)
    master_slave = parse_macrolanguages(files['macrolanguages'])
    pprint.pprint(master_slave)


NameIds = namedtuple('NameIds', ('name', 'ids', ))
MasterSlave = namedtuple('MasterSlave', ('master', 'slave', ))


def parse_index(text):
    csvreader = csv.reader(io.StringIO(text), delimiter='\t')
    iter_csvreader = iter(csvreader)
    title = next(iter_csvreader)
    assert len(title) == 8, 'index must contain 8 columns'
    index_id = title.index('Id')
    part2b_id = title.index('Part2B')
    part2t_id = title.index('Part2T')
    part1_id = title.index('Part1')
    scope_id = title.index('Scope')
    langtype_id = title.index('Language_Type')
    refname_id = title.index('Ref_Name')
    comments_id = title.index('Ref_Name')
    indices_to_test = (index_id, part2b_id, part2t_id, part1_id)
    all_ids = set()
    all_data = []
    for row in iter_csvreader:
        ids = []
        for index in indices_to_test:
            if row[index] and row[index] not in ids:
                ids.append(row[index])
        for i in ids:
            assert i not in all_ids
            all_ids.add(i)
        name_ids = NameIds(name=row[refname_id], ids=ids)
        all_data.append(name_ids)
    return all_data


def parse_macrolanguages(text):
    csvreader = csv.reader(io.StringIO(text), delimiter='\t')
    iter_csvreader = iter(csvreader)
    title = next(iter_csvreader)
    assert len(title) == 3, 'index must contain 3 columns'
    master_id = title.index('M_Id')
    slave_id = title.index('I_Id')
    status_id = title.index('I_Status')
    all_data = []
    for row in iter_csvreader:
        ms = MasterSlave(master=row[master_id], slave=row[slave_id])
        all_data.append(ms)
    print(all_data)
    return all_data


def main():
    parser = argparse.ArgumentParser('Generate iso639-3 language list')

    subparsers = parser.add_subparsers()

    fetch_parser = subparsers.add_parser('fetch', description='Fetch resource')
    fetch_parser.add_argument('-o', metavar='FILE', dest='output', default=None, required=True,
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
