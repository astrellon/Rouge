-- Text Styles
-- Default game objects.
styles = {}
local style = {}
style.nodeType = "gameobj"
style.style = { colour = "blue"}
table.insert(styles, style)

style = {}
style.nodeType = "gameobj"
style.flag = "hover"
style.style = {colour = "0.5 0.5 1"}
table.insert(styles, style)

style = {}
style.nodeType = "gameobj"
style.flag = "down"
style.style = {colour = "0.0 0.0 0.5"}
table.insert(styles, style)

-- Characters
style = {}
style.nodeType = "gameobj"
style.attributes = {class = "character"	}
style.style = {colour = "1 0 1"	}
table.insert(styles, style)

style = {}
style.nodeType = "gameobj"
style.attributes = {class = "character"}
style.flag = "hover"
style.style = {colour = "1 0.5 1"}
table.insert(styles, style)

style = {}
style.nodeType = "gameobj"
style.attributes = {class = "character"	}
style.flag = "down"
style.style = {colour = "0.5 0 0.5"}
table.insert(styles, style)

-- Items
style = {}
style.nodeType = "gameobj"
style.attributes = {class = "item"}
style.style = {colour = "1 1 0"}
table.insert(styles, style)

style = {}
style.nodeType = "gameobj"
style.attributes = {class = "item"}
style.flag = "hover"
style.style = {colour = "1 1 0.5"}
table.insert(styles, style)

style = {}
style.nodeType = "gameobj"
style.attributes = {class = "item"}
style.flag = "down"
style.style = {colour = "0.5 0.5 0"}
table.insert(styles, style)

-- Doors
style = {}
style.nodeType = "gameobj"
style.attributes = {class = "door"}
style.style = {colour = "1 0.5 0.0"}
table.insert(styles, style)

style = {}
style.nodeType = "gameobj"
style.attributes = {class = "door"}
style.flag = "hover"
style.style = {colour = "1 0.75 0.5"}
table.insert(styles, style)

style = {}
style.nodeType = "gameobj"
style.attributes = {class = "door"}
style.flag = "down"
style.style = {colour = "0.5 0.25 0"}
table.insert(styles, style)

-- Titles
style = {}
style.nodeType = "title"
style.style = {colour = "yellow"}
table.insert(styles, style)

style = {}
style.nodeType = "tile"
style.style = {colour = "0.4 0.5 0.8"}
table.insert(styles, style)

style = {}
style.nodeType = "tile"
style.flag = "hover"
style.style = {colour = "0.6 0.7 1.0"}
table.insert(styles, style)

-- Inspector text
style = {}
style.nodeType = "inspector"
style.style = {colour = "white"}
table.insert(styles, style)

-- Character screen text
style = {}
style.nodeType = "char_screen"
style.style = {colour = "white"}
table.insert(styles, style)

-- Health
style = {}
style.nodeType = "title"
style.attributes = {class = "health"}
style.style = {colour = "red"}
table.insert(styles, style)

-- Strength
style = {}
style.nodeType = "title"
style.attributes = {class = "strength"}
style.style = {colour = "orange"}
table.insert(styles, style)

-- Dexterity
style = {}
style.nodeType = "title"
style.attributes = {class = "dexterity"}
style.style = {colour = "green"}
table.insert(styles, style)

-- Constitution
style = {}
style.nodeType = "title"
style.attributes = {class = "constitution"}
style.style = {colour = "0.9 0.7 0.15"}
table.insert(styles, style)

-- Arcane
style = {}
style.nodeType = "title"
style.attributes = {class = "arcane"}
style.style = {colour = "0.9 0.15 0.85"}
table.insert(styles, style)

-- Divine
style = {}
style.nodeType = "title"
style.attributes = {class = "divine"}
style.style = {colour = "0.1 0.8 0.9"}
table.insert(styles, style)

-- Speed
style = {}
style.nodeType = "title"
style.attributes = {class = "speed"}
style.style = {colour = "0.5 0.85 0.95"}
table.insert(styles, style)

-- Damage
style = {}
style.nodeType = "title"
style.attributes = {class = "damage"}
style.style = {colour = "0.9 0. 7 0.5"}
table.insert(styles, style)

-- Armour
style = {}
style.nodeType = "title"
style.attributes = {class = "armour"}
style.style = {colour = "0.6 0.8 1.0"}
table.insert(styles, style)

-- Dialogue
style = {}
style.nodeType = "dialogue"
style.style = {colour = "0.9 1.0 0.95"}
table.insert(styles, style)

style = {}
style.nodeType = "?"
style.style = {colour = "1.0 0.4 0.8"}
table.insert(styles, style)

style = {}
style.nodeType = "?"
style.flag = "hover"
style.style = {colour = "1.0 0.6 0.9"}
table.insert(styles, style)

style = {}
style.nodeType = "x"
style.style = {colour = "0.8 1.0 0.4"}
table.insert(styles, style)

style = {}
style.nodeType = "x"
style.flag = "hover"
style.style = {colour = "0.9 1.0 0.6"}
table.insert(styles, style)