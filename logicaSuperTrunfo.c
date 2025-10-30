"""
Super Trunfo: Lógica das Redes (CLI)

Arquivo: super_trunfo_rede.py
Descrição: Jogo estilo 'Super Trunfo' sobre redes de computadores.
Linguagem: Python 3

Como jogar:
- Dois jogadores: Usuário vs Computador.
- Cada carta representa um conceito de rede com atributos numéricos.
- No início de cada rodada, o jogador na vez escolhe um atributo para competir.
- A carta do jogador e a do oponente são comparadas pelo atributo; quem tiver maior valor ganha as cartas.
- O vencedor vai para o final do seu baralho com as cartas conquistadas.
- Empates geram um 'pote' com as cartas em disputa; a próxima rodada continua com o mesmo atributo até resolução.
- O jogo termina quando um jogador fica sem cartas.

Requisitos: Python 3.x
Execute: python super_trunfo_rede.py
"""
import random
import sys

class Card:
    def __init__(self, name, attrs):
        self.name = name
        self.attrs = attrs  # dict attribute_name -> int (0-100)

    def __str__(self):
        lines = [f"{self.name}"]
        for k, v in self.attrs.items():
            lines.append(f"  {k}: {v}")
        return "\n".join(lines)

class Deck:
    def __init__(self, cards=None):
        self.cards = cards[:] if cards else []

    def shuffle(self):
        random.shuffle(self.cards)

    def draw(self):
        return self.cards.pop(0) if self.cards else None

    def add_to_bottom(self, cards):
        if isinstance(cards, list):
            self.cards.extend(cards)
        else:
            self.cards.append(cards)

    def __len__(self):
        return len(self.cards)

    def is_empty(self):
        return len(self.cards) == 0

# Define cards: name and attributes
def build_card_set():
    # Attributes: speed, reliability, scalability, security, complexity
    # Values range 0-100, higher is better
    card_definitions = [
        ("Ethernet (Cat6)", {"speed": 70, "reliability": 85, "scalability": 60, "security": 50, "complexity": 40}),
        ("Fibra Óptica", {"speed": 95, "reliability": 90, "scalability": 80, "security": 70, "complexity": 60}),
        ("Wi-Fi 6", {"speed": 80, "reliability": 65, "scalability": 70, "security": 60, "complexity": 50}),
        ("Router", {"speed": 60, "reliability": 80, "scalability": 75, "security": 55, "complexity": 65}),
        ("Switch", {"speed": 75, "reliability": 85, "scalability": 70, "security": 50, "complexity": 45}),
        ("NAT", {"speed": 60, "reliability": 70, "scalability": 65, "security": 55, "complexity": 50}),
        ("Firewall", {"speed": 55, "reliability": 80, "scalability": 60, "security": 95, "complexity": 70}),
        ("IDS/IPS", {"speed": 50, "reliability": 75, "scalability": 55, "security": 90, "complexity": 80}),
        ("TCP", {"speed": 65, "reliability": 95, "scalability": 70, "security": 60, "complexity": 50}),
        ("UDP", {"speed": 85, "reliability": 60, "scalability": 75, "security": 40, "complexity": 30}),
        ("ICMP", {"speed": 40, "reliability": 50, "scalability": 30, "security": 30, "complexity": 20}),
        ("HTTP/HTTPS", {"speed": 70, "reliability": 80, "scalability": 85, "security": 65, "complexity": 45}),
        ("DNS", {"speed": 75, "reliability": 85, "scalability": 90, "security": 50, "complexity": 55}),
        ("Cloud Data Center", {"speed": 90, "reliability": 95, "scalability": 98, "security": 80, "complexity": 85}),
        ("Peer-to-Peer", {"speed": 60, "reliability": 55, "scalability": 50, "security": 40, "complexity": 30}),
        ("Client-Server", {"speed": 70, "reliability": 80, "scalability": 85, "security": 60, "complexity": 50}),
        ("VPN", {"speed": 60, "reliability": 75, "scalability": 65, "security": 90, "complexity": 70}),
        ("Load Balancer", {"speed": 80, "reliability": 90, "scalability": 90, "security": 60, "complexity": 75}),
        ("BGP", {"speed": 50, "reliability": 85, "scalability": 95, "security": 50, "complexity": 90}),
        ("OSPF", {"speed": 55, "reliability": 85, "scalability": 80, "security": 50, "complexity": 80}),
    ]
    return [Card(name, attrs) for name, attrs in card_definitions]

class SuperTrunfoGame:
    def __init__(self, player_name="Você"):
        cards = build_card_set()
        random.shuffle(cards)
        half = len(cards) // 2
        self.player_deck = Deck(cards[:half])
        self.cpu_deck = Deck(cards[half:])
        self.pot = []  # cards in dispute during ties
        self.current_player_is_human = True
        self.player_name = player_name

    def play(self):
        round_no = 1
        print("Bem-vindo ao Super Trunfo das Redes!\n")
        while not self.game_over():
            print(f"--- Rodada {round_no} ---")
            print(f"{self.player_name} tem {len(self.player_deck)} cartas. CPU tem {len(self.cpu_deck)} cartas.")

            player_card = self.player_deck.draw()
            cpu_card = self.cpu_deck.draw()

            if not player_card or not cpu_card:
                break

            print(f"\nCarta do {self.player_name}:\n{player_card}\n")
            print(f"Carta do CPU: {cpu_card.name}\n")

            # choose attribute
            if self.current_player_is_human:
                attr = self.prompt_attribute(player_card)
            else:
                attr = self.cpu_choose_attribute(cpu_card)
                print(f"CPU escolheu competir por: {attr}\n")

            self.pot.extend([player_card, cpu_card])

            player_val = player_card.attrs[attr]
            cpu_val = cpu_card.attrs[attr]

            print(f"{self.player_name}: {player_val}  vs  CPU: {cpu_val}")

            if player_val > cpu_val:
                print(f"{self.player_name} vence a rodada!\n")
                self.player_deck.add_to_bottom(self.pot)
                self.pot = []
                self.current_player_is_human = True
            elif player_val < cpu_val:
                print("CPU vence a rodada!\n")
                self.cpu_deck.add_to_bottom(self.pot)
                self.pot = []
                self.current_player_is_human = False
            else:
                print("Empate! As cartas vão para o pote. Próxima rodada continua disputando o mesmo atributo.\n")
                # On tie, the next player remains the same (rules can vary). Keep same current player.
                # Continue to next round which will draw new cards and add to pot.

            round_no += 1
            # allow user to quit
            if round_no % 10 == 0:
                resp = input("Continuar jogando? (s/n): ").strip().lower()
                if resp != "s":
                    print("Jogo interrompido pelo usuário.")
                    break

        # end
        if len(self.player_deck) > len(self.cpu_deck):
            print(f"\n{self.player_name} venceu o jogo! Parabéns!")
        elif len(self.cpu_deck) > len(self.player_deck):
            print("\nCPU venceu o jogo. Melhor sorte na próxima!")
        else:
            print("\nJogo terminou sem vencedor claro.")

    def prompt_attribute(self, card):
        attrs = list(card.attrs.keys())
        print("Escolha um atributo para competir:")
        for i, a in enumerate(attrs, 1):
            print(f"  {i}. {a} ({card.attrs[a]})")
        while True:
            choice = input("Digite o número do atributo: ").strip()
            if not choice.isdigit():
                print("Entrada inválida. Digite o número do atributo.")
                continue
            idx = int(choice) - 1
            if 0 <= idx < len(attrs):
                return attrs[idx]
            print("Escolha fora do intervalo. Tente novamente.")

    def cpu_choose_attribute(self, card):
        # heuristic: choose attribute with highest value for CPU card
        return max(card.attrs.items(), key=lambda kv: kv[1])[0]

    def game_over(self):
        return self.player_deck.is_empty() or self.cpu_deck.is_empty()

if __name__ == '__main__':
    game = SuperTrunfoGame()
    try:
        game.play()
    except KeyboardInterrupt:
        print("\nJogo interrompido.")
        sys.exit(0)
