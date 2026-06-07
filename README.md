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
| Traitement des flows | Node-RED |
| Base de données | InfluxDB 2.0 (Cloud) |
| Microcontrôleur | Arduino (communication série) |
| API météo | OpenWeatherMap |
| Dashboard | Node-RED Dashboard (ui_gauge, ui_chart) |

## Configuration

### 1. Prérequis
- Node-RED installé
- Compte InfluxDB Cloud (ou instance locale)
- Clé API OpenWeatherMap

### 2. Import des flows

Importer les fichiers JSON dans Node-RED :
- `greenhouse-main-flows.json` — flows principaux (zones 1-4 + météo)
- `greenhouse-zone1-flows.json` — flow zone 1 (version modifiée)

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
├── greenhouse-main-flows.json    # Flows Node-RED principaux (zones 1-4 + météo)
├── greenhouse-zone1-flows.json   # Flow zone 1 (version modifiée)
├── project-report.pdf            # Rapport complet du projet
├── iot-project-report.html       # Page web du projet
└── README.md                     # Ce fichier
```

## Architecture

```
Arduino (capteurs) 
    → Serial In (Node-RED)
        → Switch (routing par type)
            ├── Température → InfluxDB (valeur idéale) → Thermostat
            ├── Humidité    → InfluxDB (valeur idéale) → Pompe
            ├── Luminosité  → InfluxDB (valeur idéale) → Éclairage
            └── Insectes    → InfluxDB (insectes connus) → Alerte

OpenWeatherMap API
    → Conditions météo
        → Contrôle dôme (ouvert si ciel dégagé)
```
