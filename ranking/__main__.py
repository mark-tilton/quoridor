import math


# Function to calculate the Probability
def Probability(rating1, rating2):
    return 1.0 * 1.0 / (1 + 1.0 * math.pow(10, 1.0 * (rating1 - rating2) / 400))


# Function to calculate ELO rating
# K is a constant.
# d determines whether Player A wins or Player B.
def EloRating(Ra, Rb, K, d):
    Pa = Probability(Rb, Ra)  # The probability of Player A winning
    Pb = Probability(Ra, Rb)  # The probability of Player B winning

    if d == 1:
        # Player A wins
        Ra = Ra + K * (1 - Pa)
        Rb = Rb + K * (0 - Pb)
    else:
        # Player B wins
        Ra = Ra + K * (0 - Pa)
        Rb = Rb + K * (1 - Pb)

    print("Updated Ratings:")
    print("Pa = {0}% | Pb = {1}%".format(round(Pa * 100, 2), round(Pb * 100, 2)))
    print("Ra = {0} | Rb = {1}".format(round(Ra, 6), round(Rb, 6)))

# Ra and Rb are current ELO ratings
Ra = 1200
Rb = 800
K = 30
d = 0
EloRating(Ra, Rb, K, d)
