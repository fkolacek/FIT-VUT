#!/bin/bash

ids=$(ipcs | grep $(whoami) | awk "{print \$2}"); for id in $ids; do ipcrm -m $id 2>/dev/null ipcrm -q $id 2>/dev/null ipcrm -s $id 2>/dev/null; done
