
base = "ATGC"
prob = [0.5, 0.25, 0.125, 0.125]

function generate_text(filename, length, base, prob)
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

generate_text("modelA.txt", 100, base, prob)