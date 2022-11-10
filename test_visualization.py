from prospr import Protein, depth_first_bnb, plot_protein

if __name__ == '__main__':
    p = Protein("HPPHPPHPPH", model="HP")
    p = depth_first_bnb(p)
    print(p.score)
    plot_protein(p)
