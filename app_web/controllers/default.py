# -*- coding: utf-8 -*-

def index():

    #verifica se foi feito uma requisição post para o servidor
    if request.post_vars:
        #seta o tipo como zero (movendo-se)
        tipo = 0
        #pega o dado vindo do servidor
        dado = int(request.post_vars.movimento)
        #verifica o valor de aceleração se for menor que 50.000
        #está se movimentando, se for maior está sentado
        if dado >= 50000:
            tipo = 1
        #grava os dados no banco
        db.movimentos.insert(movimento = dado, tipo_movimento = tipo, dia = hoje)
    
    #realiza uma consulta na base trazendo todos os dados recebidos do dia
    dados_recebidos = db((db.movimentos.id > 0) & (db.movimentos.dia == hoje)).select(
                            orderby=~db.movimentos.id, 
                            limitby=(0,20)
                            )

    #faz uma contagem dos dados do tipo 0 do dia
    movendo = db((db.movimentos.tipo_movimento == 0) & (db.movimentos.dia == hoje)).count() or 0
    #faz uma contagem dos dados do tipo 1 do dia
    parado = db((db.movimentos.tipo_movimento == 1) & (db.movimentos.dia == hoje)).count() or 0
        
    return locals()
