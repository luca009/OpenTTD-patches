#ifndef STATION_H
#define STATION_H

#include "sprite.h"
#include "vehicle.h"

typedef struct GoodsEntry {
	uint16 waiting_acceptance;
	byte days_since_pickup;
	byte rating;
	byte enroute_from;
	byte enroute_time;
	byte last_speed;
	byte last_age;
} GoodsEntry;

struct Station {
	TileIndex xy;
	TileIndex bus_tile;
	TileIndex lorry_tile;
	TileIndex train_tile;
	TileIndex airport_tile;
	TileIndex dock_tile;
	Town *town;
	// alpha_order is obsolete since savegame format 4
	byte alpha_order_obsolete;
	uint16 string_id;

	ViewportSign sign;

	uint16 had_vehicle_of_type;

	byte time_since_load;
	byte time_since_unload;
	byte delete_ctr;
	byte owner;
	byte facilities;
	byte airport_type;
	byte truck_stop_status;
	byte bus_stop_status;
	byte blocked_months_obsolete;

	// trainstation width/height
	byte trainst_w, trainst_h;

	byte class_id; // custom graphics station class
	byte stat_id; // custom graphics station id in the @class_id class
	uint16 build_date;

	//uint16 airport_flags;
	uint32 airport_flags;
	uint16 index;

	VehicleID last_vehicle;
	GoodsEntry goods[NUM_CARGO];
};

enum {
	FACIL_TRAIN = 1,
	FACIL_TRUCK_STOP = 2,
	FACIL_BUS_STOP = 4,
	FACIL_AIRPORT = 8,
	FACIL_DOCK = 0x10,
};

enum {
//	HVOT_PENDING_DELETE = 1<<0, // not needed anymore
	HVOT_TRAIN = 1<<1,
	HVOT_BUS = 1 << 2,
	HVOT_TRUCK = 1 << 3,
	HVOT_AIRCRAFT = 1<<4,
	HVOT_SHIP = 1 << 5,
	HVOT_BUOY = 1 << 6
};

enum {
	CA_BUS = 3,
	CA_TRUCK = 3,
	CA_AIR_OILPAD = 3,
	CA_TRAIN = 4,
	CA_AIR_HELIPORT = 4,
	CA_AIR_SMALL = 4,
	CA_AIR_LARGE = 5,
	CA_DOCK = 5,
	CA_AIR_METRO = 6,
	CA_AIR_INTER = 8,
};

void ModifyStationRatingAround(TileIndex tile, byte owner, int amount, uint radius);

TileIndex GetStationTileForVehicle(const Vehicle *v, const Station *st);

void ShowStationViewWindow(int station);
void UpdateAllStationVirtCoord();

VARDEF Station _stations[250];
VARDEF uint _stations_size;

VARDEF SortStruct *_station_sort;

static inline Station *GetStation(uint index)
{
	assert(index < _stations_size);
	return &_stations[index];
}

#define FOR_ALL_STATIONS(st) for(st = _stations; st != &_stations[_stations_size]; st++)

VARDEF bool _station_sort_dirty[MAX_PLAYERS];
VARDEF bool _global_station_sort_dirty;

void GetProductionAroundTiles(uint *produced, uint tile, int w, int h, int rad);
void GetAcceptanceAroundTiles(uint *accepts, uint tile, int w, int h, int rad);
uint GetStationPlatforms(Station *st, uint tile);


/* Station layout for given dimensions - it is a two-dimensional array
 * where index is computed as (x * platforms) + platform. */
typedef byte *StationLayout;

struct StationSpec {
	uint32 grfid;
	int localidx; // per-GRFFile station index + 1; SetCustomStation() takes care of this

	enum StationClass {
		STAT_CLASS_NONE, // unused station slot or so
		STAT_CLASS_DFLT, // default station class
		STAT_CLASS_WAYP, // waypoints

		/* TODO: When we actually support custom classes, they are
		 * going to be allocated dynamically (with some classid->sclass
		 * mapping, there's a TTDPatch limit on 16 custom classes in
		 * the whole game at the same time) with base at
		 * STAT_CLASS_CUSTOM. --pasky */
		STAT_CLASS_CUSTOM, // some custom class
	} sclass;

	/* Bitmask of platform numbers/lengths available for the station.  Bits
	 * 0..6 correspond to 1..7, while bit 7 corresponds to >7 platforms or
	 * lenght. */
	byte allowed_platforms;
	byte allowed_lengths;

	/* Custom sprites */
	byte tiles;
	/* 00 = plain platform
	 * 02 = platform with building
	 * 04 = platform with roof, left side
	 * 06 = platform with roof, right side
	 *
	 * These numbers are used for stations in NE-SW direction, or these
	 * numbers plus one for stations in the NW-SE direction.  */
	DrawTileSprites renderdata[8];

	/* Custom layouts */
	/* The layout array is organized like [lenghts][platforms], both being
	 * dynamic arrays, the field itself is length*platforms array containing
	 * indexes to @renderdata (only even numbers allowed) for the given
	 * station tile. */
	/* @lengths is length of the @platforms and @layouts arrays, that is
	 * number of maximal length for which the layout is defined (since
	 * arrays are indexed from 0, the length itself is at [length - 1]). */
	byte lengths;
	/* @platforms is array of number of platforms defined for each length.
	 * Zero means no platforms defined. */
	byte *platforms;
	/* @layout is @layouts-sized array of @platforms-sized arrays,
	 * containing pointers to length*platforms-sized arrays or NULL if
	 * default OTTD station layout should be used for stations of these
	 * dimensions. */
	StationLayout **layouts;

	/* Sprite offsets for renderdata->seq->image. spritegroup[0] is default
	 * whilst spritegroup[1] is "CID_PURCHASE". */
	struct SpriteGroup spritegroup[2];
};

/* Here, @stid is local per-GRFFile station index. If spec->localidx is not yet
 * set, it gets new dynamically allocated global index and spec->localidx is
 * set to @stid, otherwise we take it as that we are replacing it and try to
 * search for it first (that isn't much fast but we do it only very seldom). */
void SetCustomStation(byte stid, struct StationSpec *spec);
/* Here, @stid is global station index (in continous range 0..GetCustomStationsCount())
 * (lookup is therefore very fast as we do this very frequently). */
struct StationSpec *GetCustomStation(enum StationClass sclass, byte stid);
/* Get sprite offset for a given custom station and station structure (may be
 * NULL if ctype is set - that means we are in a build dialog). The station
 * structure is used for variational sprite groups. */
uint32 GetCustomStationRelocation(struct StationSpec *spec, struct Station *stat, byte ctype);
int GetCustomStationsCount(enum StationClass sclass);

#endif /* STATION_H */
