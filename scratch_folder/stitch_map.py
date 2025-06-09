import json
import os
from enum import Enum, IntEnum
from typing import Dict, List, Tuple, NamedTuple, Optional, Any, Set
import jinja2

additional_exits: Dict[str, List[str]] = {
    "SNOWPOINT_CITY": ["FIGHT_AREA"],
    "CANALAVE_CITY": ["IRON_ISLAND", "FULLMOON_ISLAND"],
    "CANALAVE_CITY_HARBOR_INN": ["NEWMOON_ISLAND"],
    "SPEAR_PILLAR_DISTORTED": ["DISTORTION_WORLD_1F"],
}

removed_locations: List[str] = [
    "MAP_HEADER_EMPTY",
    "MAP_HEADER_WIFI_PLAZA_ENTRANCE",
    "MAP_HEADER_UNION_ROOM",
    "MAP_HEADER_BATTLE_PARK",
    "MAP_HEADER_BATTLE_PARK_EXCHANGE_SERVICE_CORNER",
    "MAP_HEADER_BATTLE_PARK_GATE_TO_FIGHT_AREA",
    "MAP_HEADER_COMMUNICATION_CLUB_COLOSSEUM_2P",
    "MAP_HEADER_COMMUNICATION_CLUB_COLOSSEUM_4P",
    # temp disables
    "TURNBACK_CAVE_PILLAR_2_ROOM_2",
    "TURNBACK_CAVE_PILLAR_2_ROOM_3",
    "TURNBACK_CAVE_PILLAR_2_ROOM_4",
    "TURNBACK_CAVE_PILLAR_2_ROOM_5",
    "TURNBACK_CAVE_PILLAR_2_ROOM_6",
    "UNKNOWN_MAP_535",
    "UNKNOWN_MAP_540",
    "UNKNOWN_MAP_546",
    "UNDERGROUND",
    "JUBILIFE_CITY_UNKNOWN_HOUSE_1",
    "JUBILIFE_CITY_UNKNOWN_HOUSE_2",
    "ETERNA_CITY_UNKNOWN_HOUSE_1",
    "VISTA_LIGHTHOUSE",
    "UNKNOWN_MAP_197",
    "UNKNOWN_MAP_250",
    "UNKNOWN_MAP_255",
    "TURNBACK_CAVE_PILLAR_ROOM",
    "TURNBACK_CAVE_GIRATINA_ROOM",
    "BATTLE_TOWER_ELEVATOR",
    "BATTLE_TOWER_CORRIDOR",
    "BATTLE_TOWER_BATTLE_ROOM",
    "BATTLE_TOWER_MULTI_BATTLE_ROOM",
    "ETERNA_CITY_UNKNOWN_HOUSE",
    "HEARTHOME_CITY_SOUTHEAST_HOUSE_2F",
    "HEARTHOME_CITY_NORTHEAST_HOUSE_2F",
    "CONTEST_HALL_STAGE_ONGOING_CONTEST",
    "TURNBACK_CAVE_PILLAR_1_ROOM_1",
    "TURNBACK_CAVE_PILLAR_1_ROOM_2",
    "TURNBACK_CAVE_PILLAR_1_ROOM_3",
    "BATTLE_TOWER_CORRIDOR_MULTI",
    "VERITY_LAKEFRONT_UNKNOWN_HOUSE",
    "GATE_BETWEEN_ETERNA_CITY_ROUTE_206",
    "RESORT_AREA_MART",
    "RESORT_AREA_RIBBON_SYNDICATE_2F",
    "BATTLE_TOWER_BATTLE_SALON",
    "HALL_OF_ORIGIN",
    "TURNBACK_CAVE_PILLAR_1_ROOM_4",
    "TURNBACK_CAVE_PILLAR_1_ROOM_5",
    "TURNBACK_CAVE_PILLAR_1_ROOM_6",
    "TURNBACK_CAVE_PILLAR_2_ROOM_1",
    "TURNBACK_CAVE_PILLAR_3_ROOM_1",
    "TURNBACK_CAVE_PILLAR_3_ROOM_2",
    "TURNBACK_CAVE_PILLAR_3_ROOM_3",
    "TURNBACK_CAVE_PILLAR_3_ROOM_4",
    "TURNBACK_CAVE_PILLAR_3_ROOM_5",
    "TURNBACK_CAVE_PILLAR_3_ROOM_6"
]

def removed_locations_as_file_name():
    return [f"events_{header.removeprefix("MAP_HEADER_").lower()}" for header in removed_locations]

class ScriptIDOffsets(Enum):
    SCRIPT_ID_OFFSET_BERRIES = (2800, 200)
    SCRIPT_ID_OFFSET_SINGLE_BATTLE = (3000, 1000)
    SCRIPT_ID_OFFSET_DOUBLE_BATTLE = (5000, 1000)
    SCRIPT_ID_OVERWORLD_ITEMS = (7000, 1000)
    SCRIPT_ID_HIDDEN_ITEMS = (8000, 800)
    @staticmethod
    def script_id_range(script_id):
        for group in list(ScriptIDOffsets):
            if group.value[0] <= script_id < group.value[0] + group.value[1]:
                return group
        return None


def generate_warps(warp_events: List[Dict]) -> Dict:
    ret_warps: Dict = {}
    for warp_event in warp_events:
        if warp_event["dest_header_id"] == "MAP_HEADER_DYNAMIC" or "DP" in warp_event["dest_header_id"] or warp_event["dest_header_id"] in removed_locations:
            continue
        else:
            ret_warps[f"{warp_event["dest_header_id"].removeprefix("MAP_HEADER_")}:{warp_event["dest_warp_id"]}"] = 0
    return ret_warps

def generate_locations(location_events: Dict, _values_found: Set[str]) -> List:
    return list(generate_from_object(location_events["object_events"], _values_found)) + (generate_from_bg(location_events["bg_events"], _values_found))
def generate_from_object(location_events: List, _values_found: Set[str]) -> Set[str]:
    ret_locations: Set[str] = set()
    for object_event in location_events:
        script_num = int(object_event["script"])

        match ScriptIDOffsets.script_id_range(script_num):
            case ScriptIDOffsets.SCRIPT_ID_OFFSET_BERRIES:
                if len(object_event["data"]) == 0:
                    ret_locations.add("BERRY_0")
                else:
                    ret_locations.add(f"BERRY_{object_event["data"][0]}")
                counts["berry"] += 1
            case ScriptIDOffsets.SCRIPT_ID_OFFSET_SINGLE_BATTLE:
                ret_locations.add(f"{trainer_names[script_num - ScriptIDOffsets.SCRIPT_ID_OFFSET_SINGLE_BATTLE.value[0] + 1].strip()}")
            case ScriptIDOffsets.SCRIPT_ID_OFFSET_DOUBLE_BATTLE:
                ret_locations.add(
                    f"{trainer_names[script_num - ScriptIDOffsets.SCRIPT_ID_OFFSET_DOUBLE_BATTLE.value[0] + 1].strip()}")
            case ScriptIDOffsets.SCRIPT_ID_OVERWORLD_ITEMS:
                overworld_item_id = f"OVERWORLD_ITEM_{script_num - ScriptIDOffsets.SCRIPT_ID_OVERWORLD_ITEMS.value[0]}"
                idx = 1
                while overworld_item_id in _values_found:
                    overworld_item_id = f"OVERWORLD_ITEM_{script_num - ScriptIDOffsets.SCRIPT_ID_OVERWORLD_ITEMS.value[0]}_{idx}"
                    idx+=1
                ret_locations.add(overworld_item_id)
                values_found.add(overworld_item_id)
                counts["overworld_items"] += 1
    
    return ret_locations

def generate_from_bg(location_events: List, values_set: Set[str]) -> List:
    global counts
    ret_locations: List = []
    for bg_event in location_events:
        script_num = int(bg_event["script"])
        match ScriptIDOffsets.script_id_range(script_num):
            case ScriptIDOffsets.SCRIPT_ID_HIDDEN_ITEMS:
                hidden_item_id = f"HIDDEN_ITEM_{script_num - ScriptIDOffsets.SCRIPT_ID_HIDDEN_ITEMS.value[0]}"
                idx = 1
                while hidden_item_id in values_set:
                    hidden_item_id = f"HIDDEN_ITEM_{script_num - ScriptIDOffsets.SCRIPT_ID_HIDDEN_ITEMS.value[0]}_{idx}"
                    idx+=1
                values_set.add(hidden_item_id)
                ret_locations.append(hidden_item_id)
                counts["hidden_items"] += 1
    return ret_locations



class PatchType(IntEnum):
    EXIT = 0
    LOCATION = 1

class Patch(NamedTuple):
    key_id: str
    condition: Optional[str]
    patch_type: PatchType

def update_data(data: List):
    # badges
    patches: Dict[str, Patch] = {
        "OREBURGH_CITY_GYM": Patch("BADGE_1", None, PatchType.LOCATION),
        "ETERNA_CITY_GYM": Patch("BADGE_2", None, PatchType.LOCATION),
        "HEARTHOME_CITY_GYM_LEADER_ROOM": Patch("BADGE_3", None, PatchType.LOCATION),
        "VEILSTONE_CITY_GYM": Patch("BADGE_4", None, PatchType.LOCATION),
        "PASTORIA_CITY_GYM": Patch("BADGE_5", None, PatchType.LOCATION),
        "CANALAVE_CITY_GYM": Patch("BADGE_6", None, PatchType.LOCATION),
        "SNOWPOINT_CITY_GYM": Patch("BADGE_7", None, PatchType.LOCATION),
        "SUNYSHORE_CITY_GYM_ROOM_3": Patch("BADGE_8", None, PatchType.LOCATION),
        "VERITY_LAKEFRONT": Patch("EVENT_HAS_STARTER", None, PatchType.LOCATION),
        "TWINLEAF_TOWN_RIVAL_HOUSE_2F": Patch("EVENT_BERRY_LEFT_HOUSE", None, PatchType.LOCATION),

    }


    for region in data:
        if region["name"] in additional_exits:
            for dest in additional_exits[region["name"]]:
                region["exits"][dest] = 0

    for location in data:
        if location["name"] in patches:
            patch = patches[location["name"]]
            match patch.patch_type:
                case PatchType.EXIT:
                    for exit_entry in location["exits"]:
                        if exit_entry == patch.key_id:
                            location["exits"] = patch.condition
                            break
                        print("ERROR: unable to find exit")


                case PatchType.LOCATION:
                    if patch[1] is None:
                        location["locations"].append(patch[0])
                    else:
                        location["locations"].append({"location_id": patch[0], "condition": patch[1]})



def stitch_map(data: List):
    for file_name in os.listdir("res/field/maps/matrices/"):
        if file_name.endswith(".json"):
            load_matrix(data, f"res/field/maps/matrices/{file_name}")

def load_matrix(data: List, file_name: str):
    json_map_matrix = json.loads(open(file_name).read())
    json_map_header = json_map_matrix["headers"]

    # create connections from map "name" to headers.
    # if header = MAP_EVERYWHERE grab every cardinal direction in matrix thats not MAP_NONE
    # if header != MAP_EVERYWHERE grab header
    x_len = len(json_map_header)
    if x_len == 0:
        return
    y_len = len(json_map_header[0])

    cardinal = [[-1, 0], [0, 1], [1, 0], [0, -1]]
    map_chunks: Dict[str, List[Tuple[int, int]]] = {}

    # generate map groups for each section i.e. route_201
    for x in range(x_len):
        for y in range(y_len):
            name = json_map_header[x][y].removeprefix("MAP_HEADER_")
            if name != "MAP_NONE" and name != "EVERYWHERE" and "DP" not in name:
                if name not in map_chunks:
                    map_chunks[name] = []
                map_chunks[name].append((x,y))

    # grab every touching map group filtering out ones with same name as self
    group_connections: Dict[str, List[str]] = {}  # Changed type annotation
    for chunk in map_chunks:
        for x, y in map_chunks[chunk]:
            for dx, dy in cardinal:
                if 0 < x + dx < x_len and 0 < y + dy < y_len:
                    adjacent_name = json_map_header[x + dx][y + dy]
                    adjacent_name = adjacent_name.removeprefix("MAP_HEADER_")
                    if adjacent_name != chunk and adjacent_name != "MAP_NONE" and adjacent_name != "EVERYWHERE":
                        if chunk not in group_connections:
                            group_connections[chunk] = []
                        if adjacent_name not in group_connections[chunk]:
                            group_connections[chunk].append(adjacent_name)

    for region in data:
        group = group_connections.pop(region["name"], None)
        if group is not None:
            # Convert the list of adjacent names to a dictionary
            exits_dict = {name: 0 for name in group if "DP" not in name}
            region["exits"].update(exits_dict)


def traverse_regions(data: List, current: str, visited: Set[str]):
    if current in visited:
        return
    visited.add(current)

    current_region = next((region for region in data if region["name"].split(":")[0] == current), None)
    if current_region:
        for exit_name in current_region["exits"]:
            traverse_regions(data, exit_name.split(":")[0], visited)


def solve(data: List) -> Set[str]:
    visited: Set[str] = set()
    start_region = "TWINLEAF_TOWN"
    traverse_regions(data, start_region, visited)
    return visited


counts = {"berry": 0, "trainer": 0, "dex": 0, "overworld_items": 0, "hidden_items": 0}
trainer_names: List[str] = open("generated/trainers.txt").readlines()
counts["trainer"] = len(trainer_names)
json_list = {"regions": []}

template = jinja2.Template(
"""
from enum import IntEnum

class {{class_name.title()}}(IntEnum):
{% for item in items %}
     {{item.upper().strip()}} = {{loop.index}}
{%- endfor -%}
""")

with open("res/field/events/zone_event.order") as f:
    values_found: Set[str] = set()
    for i, event_file_name in enumerate(f.readlines()):
        if "dp" in event_file_name or event_file_name.strip() in removed_locations_as_file_name():
            continue
        with open(f"res/field/events/{event_file_name.strip()}.json") as event_f:

            json_obj: Dict = json.load(event_f)
            out_json_obj: Dict = {"id": i,
                                  "name": f"{event_file_name.removeprefix("events_").upper().strip()}",
                                  "locations": generate_locations(json_obj, values_found),
                                  "exits": generate_warps(json_obj["warp_events"])}
            json_list["regions"].append(out_json_obj)



with open("scratch_folder/out/trainers.py", "w+") as f:
    f.write(template.render(class_name="Trainer", items=trainer_names))
with open("scratch_folder/out/region_data.json", "w+") as out_f:
    update_data(json_list["regions"])
    stitch_map(json_list["regions"])
    out_f.write(json.dumps(json_list, indent=4) + "\n")
    regions_ = json_list["regions"]
    reachable_regions = solve(regions_)
    required_regions = {loc.split(":")[0] for region in regions_ for loc in region["exits"] if loc.split(":")[0]}
    required_regions = list(required_regions)
    reachable_regions = list(reachable_regions)
    reachable_regions.sort()
    required_regions.sort()
    diff = [x for x in required_regions if x not in reachable_regions]
print(counts)

# find distributions
# hidden
with open("include/data/field/hidden_items.h", "r") as f:
    for i in range(20):
        f.readline()
    item_name_to_script = {}
    item_name_to_count = {}
    for i in range(257):
        raw_tuple = f.readline().strip().removeprefix("HIDDEN_ITEM_ENTRY")[1:-2].replace(" ", "").split(",")
        item_name_to_script[raw_tuple[0]] = raw_tuple[3]
        item_name_to_count[raw_tuple[0]] = item_name_to_count.get(raw_tuple[0], 0) + 1
    print(item_name_to_script)
    print(item_name_to_count)