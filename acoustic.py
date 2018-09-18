# import values
import sys

notes = ["C6", "D6", "E6", "F6", "G6", "A6", "B6", "C7"]
frequencies = [1046.5, 1174.66, 1318.51, 1396.91, 1567.98, 1760, 1975.53, 2093]

prescalers = [8, 64, 256, 1024]

arduino_hz = 16000000

verbose = False

def comp_match_register(freq, prescaler):
	return (arduino_hz/ (prescaler * freq)) - 1

def freq_calc(prescaler, match):
  return (arduino_hz/ (prescaler * (match + 1)))

comp_reg_results = []

for frequency in frequencies:
  targets =[]
  difs = []

  for prescale in prescalers:

    match_register = comp_match_register(frequency, prescale)
    corrected_target = round(match_register)
    # if (corrected_target < 0)

    corrected_freq = freq_calc(prescale, corrected_target)
    dif = abs(frequency - corrected_freq)

    if corrected_target < 255:
      difs.append(dif)
      targets.append(corrected_target)
    else :
      difs.append(10000000)
      targets.append(0)

    if verbose:
      print("frequency: ", frequency)
      print("prescaler: ", prescale)
      print("match register: ", match_register)
      print("rounded: ", corrected_target)
      print("new frequency: ", corrected_freq)
      print("difference: ", dif)

      print()

  min_idx = difs.index(min(difs))
  print("[", frequency, "]best prescaler: ", prescalers[min_idx], " counter: ", targets[min_idx])
  comp_reg_results.append(targets[min_idx])


print(comp_reg_results)
print()

for i, fr in enumerate(frequencies):
  print("#define "+ notes[i] + "_OCR", " ", comp_reg_results[i])



