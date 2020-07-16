include("utility.jl")

base = "ATGC"
prob1 = [0.5, 0.25, 0.125, 0.125]
prob2 = [0.25, 0.25, 0.25, 0.25]
prob3 = [0.125, 0.125, 0.25, 0.5]

function generate_modelA(filename, length, base, prob)
    for i = 2:size(prob,1)
        prob[i] += prob[i-1]
    end
    # print(prob)

    f = open("data/$filename", "w")
    write(f, "$length\n")
    for i = 1:length
        cur = rand()
        # println(cur)
        j = 1
        while cur > prob[j]
            j += 1
        end
        write(f, "$(base[j])")
    end
end

function generate_modelB(filename, length, base)
    
end

generate_modelA("modelC.txt", 1000, base, prob2)

println("loss for modelA with compressor 1: ", kl_sdistance(prob2, prob1))
println("loss for modelA with compressor 1: ", kl_sdistance(prob2, prob2))
println("loss for modelA with compressor 1: ", kl_sdistance(prob2, prob3))
