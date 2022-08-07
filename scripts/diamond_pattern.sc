fn main()
  let max_height 10

  let increment true
  let index 0

  loop
    if (index == max_height && increment)
      increment = false
    end
    elseif (index == 0 && !increment)
      break
    end

    let temp_idx 0
    loop
      if (temp_idx > max_height - index)
        break
      end
      print(" ")
      temp_idx ++
    end

    let star 0
    loop
      if (star == index * 2)
        break
      end

  
      print("*")

      star ++
    end
    print("\n")

    if (increment)
      index ++
    end
    else
      index --
    end
  end
endf
