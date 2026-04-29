# Qt RSS Reader
A desktop RSS reader built with **C++ and Qt**, designed to fetch, parse, and display RSS/Atom feeds in a clean and responsive interface.

## Features
* Add and manage RSS feed resources
* Fetch feeds over the network
* Parse RSS/Atom XML data
* Display articles in a structured UI (Qt Model/View)
* Open articles in the browser
* Timestamp-based feed updates
* Support for multiple feeds
* Simple and responsive Qt Widgets interface

## Tech Stack
* C++
* Qt (Qt Widgets, Qt Network, Qt XML)
* Model/View architecture (`QTableView`, models, delegates)

## How It Works
1. A feed URL is added by the user
2. The app fetches XML data using Qt Network
3. The XML is parsed using `QXmlStreamReader`
4. Parsed items are stored in models
5. Data is displayed using Qt views

Qt's event-driven networking allows asynchronous fetching without blocking the UI

## Build Instructions

### Requirements
* Qt 6
* qmake
* C++17 compatible compiler

### Build (qmake)
```console
git clone https://github.com/ignabelitzky/qt-rss-reader.git
cd qt-rss-reader/QtRSSReader
mkdir build
cd build
qmake ../QtRSSReader.pro
make
```

### Run
```console
./QtRSSReader
```

## Inspiration
This project follows the traditional structure of RSS readers built with Qt, where feeds are fetched, parsed, and displayed using models and views, similar to other Qt-based
readers in the ecosystem.

## License
This project is licensed under the [GNU General Public License v3.0](LICENSE). You can find the full text of the license here [LICENSE](LICENSE).

## Author
Ignacio Belitzky
GitHub: https://github.com/ignabelitzky

