# VirtualDjSetPro

C++/Qt6 Desktop-App für ein virtuelles DJ-Set mit:

- Fullscreen-Dark-UI
- 2 lokale Decks für MP3/WAV/FLAC
- Cue-Punkte
- A/B-Loops
- Crossfader
- Playlist-Import
- Auto-Mix mit Überblendung
- YouTube-Suche + Preview
- ChatGPT-Set-Planer mit JSON-Import/-Export
- Einstellungsdialog für OpenAI- und YouTube-API-Keys

## Wichtige technische Grenze

Diese App trennt **lokales DJ-Mixing** und **YouTube-Preview** bewusst:

- Lokale Dateien laufen über die Audio-Engine der App.
- YouTube wird nur offiziell gesucht und im eingebetteten Player angezeigt.
- Kein Audio-Extrahieren aus YouTube.
- Keine samplegenaue DJ-Kontrolle für YouTube.

Das ist absichtlich so umgesetzt.

## Build unter Windows mit VS Code + CMake + vcpkg

### 1. Voraussetzungen

- Visual Studio Build Tools oder Visual Studio mit C++
- CMake
- Ninja
- vcpkg
- Qt6 via vcpkg

### 2. vcpkg einrichten

```powershell
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
```

### 3. Projekt konfigurieren

```powershell
cd <Projektordner>
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### 4. Build

```powershell
cmake --build build
```

### 5. Start

```powershell
.\build\VirtualDjSetPro.exe
```

## VS Code

Im ZIP sind einfache VS-Code-Konfigurationsdateien enthalten. Passe bei Bedarf den vcpkg-Pfad an.

## ChatGPT-Set-Format

Die Set-Erstellung erwartet JSON. Beispiel:

```json
{
  "title": "Late Night House Warmup",
  "targetBpm": 124,
  "tracks": [
    {
      "source": "local",
      "title": "Track A",
      "artist": "Artist A",
      "bpm": 124,
      "cueMs": 15000,
      "loopInMs": 32000,
      "loopOutMs": 40000,
      "transition": "filter_fade"
    },
    {
      "source": "youtube",
      "query": "deep house warmup 124 bpm",
      "transition": "echo_out"
    }
  ]
}
```

## Projektstatus

Diese Version ist absichtlich als **große funktionale Desktop-Basis** gebaut.
Für echtes Profi-DJ-Verhalten fehlen später noch:

- Time-Stretching ohne Tonhöhenänderung
- BPM-Erkennung aus Audio
- Beatgrid
- Waveforms
- Quantize / Sync
- MIDI-Controller
- Effekte
- Recording
- echte Browser-Datenbank / Library

Die Architektur ist schon darauf vorbereitet.
