#!/bin/bash

find results/ -type f -name "*revok*" | while read line; do
  sed -i -e '/"site_detail_section/,$d' "$line";

done
