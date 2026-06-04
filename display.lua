format_version = "1.0"

-- Helper function to split the serialized string from C++
local function SplitString(inputstr, sep)
    if sep == nil then sep = "%s" end
    local t = {}
    for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
        table.insert(t, str)
    end
    return t
end

-- Helper function to draw a 16-sided circle
local function DrawCircle(x, y, radius, r, g, b, alpha)
    local points = {}
    local segments = 16
    for i = 0, segments do
        local angle = (i / segments) * math.pi * 2
        local px = x + (math.cos(angle) * radius)
        local py = y + (math.sin(angle) * radius)
        table.insert(points, {x = px, y = py})
    end
    
    -- Draw expanding rings with fading alpha
    jbox_display.draw_lines(points, {r=r, g=g, b=b, a=alpha}, "closed", 4.0, "round", "round")
end

-- --- MAIN DRAW FUNCTION ---
function ReflectionPool_Draw(property_values, display_info, dirty_rect)
    local width = display_info.width  -- 1395
    local height = display_info.height -- 555
    local midX = width / 2.0
    
    local dataString = property_values[1]
    if not dataString or dataString == "" then return end
    
    local commands = SplitString(dataString, "|")
    
    for _, cmd in ipairs(commands) do
        local type = string.sub(cmd, 1, 1)
        
        -- DRAW SINE WAVE MIRROR
        if type == "W" then
            local phase = tonumber(string.sub(cmd, 3))
            if phase then
                local wavePoints = {}
                local segments = 80 -- Increased segments for a smoother wave at full 1395px resolution
                for i = 0, segments do
                    local py = (i / segments) * height
                    local px = midX + (math.sin((i / segments) * math.pi * 4.0 + phase) * 25.0)
                    table.insert(wavePoints, {x = px, y = py})
                end
                -- Dark Blue Sine Wave (20.0 thickness)
                jbox_display.draw_lines(wavePoints, {r=0, g=50, b=150, a=200}, "open", 20.0, "round", "round")
            end
        
        -- DRAW INPUT RIPPLES (Left Side)
        elseif type == "I" then
            local args = SplitString(string.sub(cmd, 3), ",")
            if #args == 4 then
                -- Push them away from the edges and the center sine wave to prevent bleeding
                local usableWidth = midX - 150
                local nx = 75 + (tonumber(args[1]) * usableWidth)
                
                local ny = tonumber(args[2]) * height
                local radius = tonumber(args[3])
                local alpha = tonumber(args[4])
                
                if nx and ny and radius and alpha then
                    DrawCircle(nx, ny, radius, 255, 255, 255, alpha)
                    DrawCircle(nx, ny, radius * 0.6, 255, 255, 255, alpha * 0.6)
                    DrawCircle(nx, ny, radius * 0.3, 255, 255, 255, alpha * 0.3)
                end
            end
            
        -- DRAW OUTPUT RIPPLES (Right Side)
        elseif type == "O" then
            local args = SplitString(string.sub(cmd, 3), ",")
            if #args == 4 then
                -- Push them to the right, safely away from the center mirror
                local usableWidth = midX - 150
                local nx = midX + 75 + (tonumber(args[1]) * usableWidth)
                
                local ny = tonumber(args[2]) * height
                local radius = tonumber(args[3])
                local alpha = tonumber(args[4])
                
                if nx and ny and radius and alpha then
                    DrawCircle(nx, ny, radius, 255, 255, 255, alpha)
                    DrawCircle(nx, ny, radius * 0.6, 255, 255, 255, alpha * 0.6)
                    DrawCircle(nx, ny, radius * 0.3, 255, 255, 255, alpha * 0.3)
                end
            end
        end
    end
end

-- Force continuous repainting for animation
function ReflectionPool_Invalidate(property_values, last_property_values, display_info)
    jbox_display.invalidate({left = 0, top = 0, right = display_info.width, bottom = display_info.height})
end
