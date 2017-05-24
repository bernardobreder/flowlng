
function a() do
    return 1
end

function b() do
    return a()*2
end

return "a" + "b" + b()
