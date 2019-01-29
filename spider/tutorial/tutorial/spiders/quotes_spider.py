# -*- coding: utf-8 -*-
import scrapy


class QuotesSpider(scrapy.Spider):
    name = 'quotes'
    def start_requests(self):
        urls = [
                'http://quotes.toscrape.com/page/1/',
                ]
        for url in urls:
            yield scrapy.Request(url=url, callback=self.parse)

    def parse(self, response):
        quotes = response.css(r'div.quote')
        for quote in quotes:
            yield{
                'text': quote.css(r'span.text::text').extract_first(),
                'author' : quote.css(r'small.author::text').extract_first(),
                'tags' : quote.css(r'a.tag::text').extract(),
            } 
#        nextpage = response.css(r'li.next a::attr(href)').extract_first()
#        if nextpage is not None:
#            nextpage = response.urljoin(nextpage)
#            yield scrapy.Request(nextpage, self.parse)
        for a in response.css('li.next a'):
            yield response.follow(a, self.parse)
