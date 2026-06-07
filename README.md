# Smart Green House - Serre Intelligente IoT

Système de surveillance et d'automatisation d'une serre agricole basé sur Node-RED, Arduino et InfluxDB.

## Description

Ce projet implémente une serre intelligente avec **4 zones indépendantes** de capteurs. Chaque zone surveille les conditions environnementales en temps réel et déclenche automatiquement les actionneurs nécessaires.

### Fonctionnalités

- Surveillance température, humidité du sol, luminosité, détection d'insectes
- Comparaison aux valeurs idéales par type de plante (via InfluxDB)
- Activation automatique : thermostat, pompe à eau, éclairage
- Alertes en cas de détection d'insectes nuisibles
- Intégration météo OpenWeatherMap + contrôle automatique du dôme
- Dashboard Node-RED avec jauges, graphiques et thermostat interactif

## Stack technique

| Composant | Technologie |
|---|---|
| Microcontrôleur | Arduino Mega (C++ / Arduino IDE) |
| Traitement des flows | Node-RED |
| Base de données | InfluxDB 2.0 (Cloud) |
| API météo | OpenWeatherMap |
| Dashboard | Node-RED Dashboard (ui_gauge, ui_chart) |

## Configuration

### 1. Prérequis
- Node-RED installé
- Compte InfluxDB Cloud (ou instance locale)
- Clé API OpenWeatherMap

### 2. Import des flows

Importer le fichier JSON dans Node-RED :
- `flux_node_red.json` — flows principaux (zones + météo + dashboard)

### 3. Variables à configurer

Après import, configurer les noeuds suivants :

**InfluxDB** (noeud `Smart Green House`) :
```
URL    : YOUR_INFLUXDB_URL
Token  : YOUR_INFLUXDB_TOKEN
Org    : YOUR_INFLUXDB_ORG
Bucket : final / choix
```

**OpenWeatherMap** (noeud `weather`) :
```
API Key : YOUR_OPENWEATHERMAP_API_KEY
City    : votre ville
Country : votre pays
```

## Structure du projet

```
├── Final_Code.cpp                # Code Arduino (capteurs + actionneurs)
├── flux_node_red.json            # Flows Node-RED (zones + météo + dashboard)
├── project-report.pdf            # Rapport complet du projet
├── iot-project-report.html       # Page web du projet
└── README.md                     # Ce fichier
```

## Architecture

```
Arduino Mega (Final_Code.cpp)
    Capteurs : DHT11 (air + sol), photorésistance, PIR + son
    Actionneurs locaux : ventilateur, LEDs, servo (dôme), stepper (irrigation)
    → Serial (CSV) : temp,hum_air,hum_sol,luminosité,bugs,dome,irrigation\n

Node-RED (flux_node_red.json)
    → Serial In → Function (parser CSV)
        → Switch (routing par topic)
            ├── temperature  → InfluxDB (valeur idéale) → Thermostat
            ├── soilHumidity → InfluxDB (valeur idéale) → Pompe
            ├── luminosity   → InfluxDB (valeur idéale) → Éclairage
            └── insect       → InfluxDB (nuisibles connus) → Alerte

OpenWeatherMap API
    → Conditions météo → Contrôle dôme (ouvert si ciel dégagé)
```
