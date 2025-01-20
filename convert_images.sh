#!/bin/bash

# Controleren of de vereiste directories bestaan
if [ ! -d "./images" ]; then
  echo "De map './images' bestaat niet. Maak deze aan en voeg je afbeeldingen toe."
  exit 1
fi

if [ ! -d "./data" ]; then
  echo "De map './data' bestaat niet. Aanmaken..."
  mkdir -p ./data
fi

WIDTH=960     # Breedte van het scherm (EPD_WIDTH)
HEIGHT=540    # Hoogte van het scherm (EPD_HEIGHT)

# Bestanden verwerken in ./images
for img in ./images/*; do
  if [[ -f "$img" ]]; then
    filename=$(basename -- "$img")
    rawname="${filename%.*}.jpg"
    echo "Converteer $img naar ./data/$rawname ..."

    # Converteer afbeelding naar RAW met 4-bit grijswaarden
    #magick convert "$img" -resize ${WIDTH}x${HEIGHT}! -depth 4 gray:"./data/$rawname"

    # magick "$img"  -resize ${WIDTH}x${HEIGHT}! -crop ${WIDTH}x${HEIGHT}! -gamma 1.2 -quality 50 "./data/$rawname"
    # magick "$img"  -resize ${WIDTH}x${HEIGHT}! -crop ${WIDTH}x${HEIGHT}! -dither FloydSteinberg -gamma 1.2 -quality 50 "./data/$rawname"
    magick "$img"  -resize ${WIDTH}x${HEIGHT}! -crop ${WIDTH}x${HEIGHT}!  -brightness-contrast 10x10  -quality 50  "./data/$rawname"

    if [ $? -eq 0 ]; then
      echo "Succesvol geconverteerd: ./data/$rawname"
    else
      echo "Fout bij conversie: $img"
    fi
  fi
done

echo "Conversie voltooid. Alle bestanden zijn opgeslagen in ./data."