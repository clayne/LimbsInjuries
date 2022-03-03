
def main():
    skeletons = {}
    known_skeletons = [
        "actors\\canine\\character assets wolf\\skeleton.nif",
        "actors\\character\\character assets\\skeleton.nif",
        "actors\character\character assets female\skeleton_female.nif",
        "actors\\draugr\\character assets\\skeleton.nif",
        "actors\\draugr\\character assets\\skeletonf.nif",
        "actors\\dragon\\character assets\\skeleton.nif",
        "actors\\deer\\character assets\\skeleton.nif",
        "actors\\canine\\character assets dog\\skeleton.nif",
        "actors\\spriggan\\character assets\\skeleton.nif",
        "actors\\dragonpriest\\character assets\\skeleton.nif",
        "actors\\wisp\\character assets\\skeleton.nif",
        "actors\\horse\\character assets\\skeleton.nif",
        "actors\\icewraith\\character assets\\skeleton.nif",
        "actors\\ambient\\chicken\\character assets\\skeleton.nif",
        "actors\\ambient\\hare\\character assets\\skeleton.nif",
        "actors\\werewolfbeast\\character assets\\skeleton.nif",
        "actors\\giant\\character assets\\skeleton.nif",
        "actors\\dlc02\\benthiclurker\\character assets\\skeleton.nif",
        "actors\\skeever\\character assets\\skeleton.nif",
        "actors\mudcrab\character assets\skeleton.nif",
        "actors\\draugr\\character assets\\skeletons.nif",
        "actors\\chaurus\\character assets\\skeleton.nif",
        "actors\witchlight\character assets\skeleton.nif",
        "actors\\goat\\character assets\\skeleton.nif",
        "actors\\frostbitespider\\character assets\\skeleton.nif",
        "actors\\cow\\character assets\\skeleton.nif",
        "actors\\character\\character assets\\skeletonbeast.nif",
        "actors\\character\\character assets female\\skeletonbeast_female.nif",
        "actors\\troll\\character assets\\skeleton.nif",
        "actors\\slaughterfish\\character assets\\skeleton.nif",
        "actors\\mammoth\\character assets\\skeleton.nif",
        "actors\\horker\\character assets\\skeleton.nif",
        "actors\\sabrecat\\character assets\\skeleton.nif",
        "actors\\hagraven\\character assets\\skeleton.nif",
        "actors\\atronachstorm\\character assets\\skeleton.nif",
        "actors\\atronachfrost\\character assets\\skeleton.nif",
        "actors\\atronachflame\\character assets\\skeleton.nif",
        "actors\\falmer\\character assets\\skeleton.nif",
        "actors\\dwarvenspider\\character assets\\skeleton.nif",
        "actors\\dwarvenspherecenturion\\character assets\\skeleton.nif",
        "actors\\dwarvensteamcenturion\\character assets\\skeleton.nif",
        "actors\\dlc02\\dwarvenballistacenturion\\character assets\\skeleton.nif",
        "actors\\bear\\character assets\\skeleton.nif",
        "actors\\dlc01\\chaurusflyer\\character assets\\skeleton.nif",
        "actors\\vampirelord\\character assets\\skeleton.nif",
        "actors\\dlc01\\vampirebrute\character assets\\skeleton.nif",
        "actors\\vampireLord\\character assets\\skeleton.nif",
        "actors\\dlc02\\hmdaedra\\character assets\\skeleton.nif",
        "actors\\dlc02\\scrib\\character assets\\skeleton.nif",
        "actors\\dlc02\\netch\\characterassets\\skeleton.nif",
        "actors\\dlc02\\riekling\\character assets\\skeleton.nif",
        "actors\\dlc02\\boarriekling\\character assets\\skeleton.nif",
        "actors\\dlc02\\boarriekling\\character assets\\skeletonboar.nif"
    ]
    with open('skeletons.txt', 'r', encoding='utf-8') as inp:
        for line in inp:
            line = line.strip()
            example, race, racename, skeleton = line.split('$')
            skeletons[skeleton] = (example, race, racename)
    
    with open('new_skeletons.txt', 'w', encoding='utf-8') as outp:
        for k in skeletons:
            example, race, racename = skeletons[k]
            if k.lower() not in known_skeletons:
                outp.write(f'{skeletons[k]} {k}\n')
    
    with open('e:/MO2_Data/mods/LimbsInjures/f314li.txt', 'w', encoding='utf-8') as outp:
        for k in skeletons:
            example, race, racename = skeletons[k]
            outp.write(f'player.placeatme {example}\n')

main()
