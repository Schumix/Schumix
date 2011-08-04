/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2010-2011 Megax <http://www.megaxx.info/>
 * 
 * Schumix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Schumix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Schumix.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SCHUMIX_CONFIG_H
#define _SCHUMIX_CONFIG_H

/*
 * Aktuális verzió száma.
 */

#define revision "4.2.2"

/*
 * Program fájlainak elnevezése. Ha cserélve van név egy helyen keljen csak cserélni.
 * Alapértelemzés: Schumix
 */

#define Elnevezes "schumix"

/*
 * Debug mod. Ha aktiválva van láthatok a betöltési folyamatok stb.
 * Alapértelmezés: kikapcsolva
 */

//#define _DEBUG_MOD

/*
 * Config debug. Ha aktiválva van láthatoak mik töltödnek be.
 * Alapértelmezés: kikapcsolva
 */

//#define _CONFIG_DEBUG

/*
 * Svn multithread maximáls száma. Meghatározza mennyi lehet a max multithread.
 * Alapértelemzés: 10
 */

#define MaxSvnID 10

/*
 * Git multithread maximáls száma. Meghatározza mennyi lehet a max multithread.
 * Alapértelemzés: 10
 */

#define MaxGitID 10

/*
 * Hg multithread maximáls száma. Meghatározza mennyi lehet a max multithread.
 * Alapértelemzés: 10
 */

#define MaxHgID 10

#endif
